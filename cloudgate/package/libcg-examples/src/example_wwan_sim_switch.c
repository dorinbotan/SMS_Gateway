/* EXAMPLES:
 *  cg_wwan, cg_device
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>
#include <libcg/cg_device.h>

typedef struct {
	/** The mutex variable */
	pthread_mutex_t mutex;
	/** The condition variable */
	pthread_cond_t cond;
} cg_device_cond_t;

static void device_notification_cb(cg_device_t *device, cg_device_event_t evt, void *context) {
	cg_status_t stat;
	cg_wwan_sim_switch_mode_t mode;
	cg_wwan_sim_switch_state_t current;
	cg_device_cond_t *lock = context;
	char *wwan_dev_name;

	if(!device || !lock) return;

	//Get the active wwan device
	stat = cg_wwan_get_active_primary_dev(&wwan_dev_name);
	if(stat != CG_STATUS_OK ) {
		return;
	}

	if(strcmp(device->device_name, wwan_dev_name) == 0) {
		if(evt == CG_DEV_EVT_ARRIVED) {
			printf("Modem arrived\n\n");

			stat = cg_wwan_get_sim_switch(&mode, &current);
			if (stat != CG_STATUS_OK) {
				printf ("Could not get sim switch mode\n");
			}

			printf("Sim Switch Mode: %d\n", mode);
			printf("Sim Switch State: %d\n", current);

			if(pthread_mutex_lock(&lock->mutex) != 0) {
				printf("mutex issue\n");
				return;
			}

			if(pthread_cond_signal(&lock->cond) != 0) {
				printf("signal issue\n");
				return;
			}

			if(pthread_mutex_unlock(&lock->mutex) != 0) {
				printf("mutex issue\n");
				return;
			}
		}
	}
}

int
main(int argc, char **argv)
{
	int32_t level;
	cg_status_t stat;
	cg_device_cond_t lock;
	//We only want notifications about the network devices
	uint32_t device_mask = CG_DEVTYPE_NETWORK;

	cg_init("Wwan Sim Switch Example");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if(pthread_mutex_init(&lock.mutex, NULL) != 0) {
		printf("mutex init failed\n");
		return 1;
	}

	if(pthread_cond_init(&lock.cond, NULL) != 0) {
		printf("cond init failed\n");
		return 1;
	}

	if(pthread_mutex_lock(&lock.mutex) != 0) {
		printf("mutex issue\n");
		return 1;
	}

/**
 * Register a device_notification_cb which will be notified of every networkdevice event.
 * In this case we are interested in the arrived event of the modem, after the modem reboot.
 */
	cg_device_register_notification(device_mask, device_notification_cb, &lock);

	stat = cg_wwan_set_sim_switch(CG_SIM_SWITCH_MODE_SIM2);
	if (stat != CG_STATUS_OK) {
		printf ("Could not set sim switch mode\n");
		goto error;
	}
	printf ("Sim switch mode set sim 2 \n\n");

	stat = cg_wwan_modem_reboot();
	if (stat != CG_STATUS_OK) {
		printf ("Modem reboot failed\n");
		goto error;
	}
	printf ("Modem reboot triggered\n");
	printf ("Waiting for the modem to come back...\n\n");

	if(pthread_cond_wait(&lock.cond, &lock.mutex) != 0) {
		printf("wait_issue\n");
		goto error;
	}

	if(pthread_mutex_unlock(&lock.mutex) != 0) {
		printf("mutex_issue\n");
		goto error;
	}

	if(pthread_cond_destroy(&lock.cond) != 0) {
		printf("destroy cond issue\n");
		goto error;
	}

	if(pthread_mutex_destroy(&lock.mutex) != 0) {
		printf("destroy cond issue\n");
		goto error;
	}

	cg_deinit();

	return 0;
error:
	pthread_mutex_unlock(&lock.mutex);
	pthread_cond_destroy(&lock.cond);
	pthread_mutex_destroy(&lock.mutex);
	cg_deinit();
	return 1;
}
