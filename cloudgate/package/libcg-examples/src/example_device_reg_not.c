/* EXAMPLE: cg_device_reg_not */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_device.h>

static void device_notification_cb (cg_device_t *device, cg_device_event_t evt, void *context)
{
	switch (evt) {
	case CG_DEV_EVT_ARRIVED:
	printf ("Event to indicate that a device has arrived\n");
	break;
	case CG_DEV_EVT_REMOVED:
	printf ("Event to indicate that a device has been removed\n");
	break;
	case CG_DEV_EVT_CLAIMED:
	printf ("Event to indicate that a device has been claimed by a process\n");
	break;
	case CG_DEV_EVT_RELEASED:
	printf ("Event to indicate that a device has been released by a process and can now be claimed again\n");
	break;
	default:
	break;
	}
}

int main(void)
{
	cg_status_t stat;

	int32_t level;
	uint32_t device_mask = CG_DEVTYPE_NETWORK;
	/* CG_DEVTYPE_NETWORK | CG_DEVTYPE_SERIAL |
	   CG_DEVTYPE_DAC | CG_DEVTYPE_GPIO; */

	cg_init("example_device_reg_not");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	stat = cg_device_register_notification (device_mask, device_notification_cb, NULL);

	switch (stat) {
	case CG_STATUS_OK:
		printf ("Press any key to exit\n");
		getchar();
		break;
	case CG_STATUS_RESOURCE_BUSY:
		printf ("Resource busy\n");
		break;
	default:
	case CG_STATUS_ERROR:
		printf ("Error\n");
		break;
	}


	cg_device_deregister_notification ();
	cg_deinit();
	return 0;
}
