/* EXAMPLES:
 *  cg_wwan_upload_prl
 *  cg_wwan_get_prl_version
 *  cg_wwan_set_prl_network_update
 *  cg_wwan_get_prl_network_update
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>
#include <libcg/cg_device.h>

sem_t sem;

void printStatus(cg_status_t status)
{
	switch (status) {
		case CG_STATUS_OK:
			printf("CG_STATUS_OK\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("CG_STATUS_INVALID_PARAMETER\n");
			break;

		case CG_STATUS_ERROR:
			printf("CG_STATUS_ERROR\n");
			break;

		default:
			printf("No valid status returned\n");
	}
}

static void device_notification_cb (cg_device_t *device, cg_device_event_t evt, void *context)
{
	//If the modem is back online, release the semaphore.
	if (strstr(device->device_name, "usb") != NULL) {
		if (evt == CG_DEV_EVT_ARRIVED) {
			sleep(4); //Initialising the modem takes a few seconds.
			printf("Modem back online and initialized\n");
			sem_post(&sem);
		}
	}
}

int
main(int argc, char **argv)
{
	int32_t level;
	cg_status_t status;
	uint16_t prl_version;
	uint8_t enabled;
	char *dev_name = NULL;

	cg_init("WWAN examples");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	if (sem_init(&sem, 0, 0) == -1){
		printf("sem_init error\n");
	} else {
		status = cg_device_register_notification(CG_DEVTYPE_NETWORK, device_notification_cb ,NULL);

		if (status != CG_STATUS_OK) {
			printf("Could not register for device notifications");
			printStatus(status);
		} else {
			printf("Succesfully registered for device notifications\n");
		}

		status = cg_wwan_upload_prl(dev_name);

		if (status != CG_STATUS_OK) {
			printf("Could not upload the prl: ");
			printStatus(status);
		} else {
			printf("Prl successfully uploaded\n");
			printf("Restarting modem. Waiting ...\n");

			sem_wait(&sem);
		}

		status = cg_wwan_get_prl_version(dev_name, &prl_version);

		if (status != CG_STATUS_OK) {
			printf("Could not retrieve the prl version: ");
			printStatus(status);
		} else {
			printf("Prl version: %d\n", prl_version);
		}

		enabled = 1;
		status = cg_wwan_set_prl_network_update(dev_name, enabled);

		if (status != CG_STATUS_OK) {
			printf("Could not set PRL network update\n");
		} else {
			printf("Succesfully set PRL network update\n");
		}

		status = cg_wwan_get_prl_network_update(dev_name, &enabled);

		if (status != CG_STATUS_OK) {
			printf("Could not retrieve PRL network update\n");
		} else {
			printf("Succesfully retrieved PRL network update: %s\n", enabled ? "enabled" : "disabled");
		}

		cg_device_deregister_notification();
		sem_destroy(&sem);
	}

	free(dev_name);
	cg_deinit();

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}
