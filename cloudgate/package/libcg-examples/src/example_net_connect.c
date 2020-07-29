/* EXAMPLE:
 *  cg_net_connect
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>
#include <libcg/cg_wwan.h>

char *dev_name = NULL;

static void
tear_down (void)
{
	cg_net_deregister_interface_events (dev_name);
	cg_net_deregister_internet_events ();
	cg_deinit();
	free(dev_name);
}

static void
internet_status_callback (cg_net_internet_status_t status)
{
	switch (status) {
	case CG_NET_INTERNET_DISCONNECTED:
		printf ("The device is disconnected\n");
		break;
	case CG_NET_INTERNET_CONNECTING:
		printf ("The device is trying to set up an internet connection on an interface\n");

		break;
	case CG_NET_INTERNET_CONNECTED:
		printf ("The device is connected to the internet\n");

		break;
	case CG_NET_INTERNET_ON_DEMAND:
		printf ("The device has found a WAN interface to connect to, but the interface is configured to connect in on-demand mode, which means that the interface will only be connected when there is traffic outgoing\n");
		break;
	}
}

static void interface_event_callback (const char *dev_name, cg_net_if_status_t status)
{
	printf ("Interface event on %s %s%s%s%s\n", dev_name,
		status & CG_NET_INTERFACE_ENABLED ? "enabled " : " ",
		status & CG_NET_INTERFACE_CONFIGURED ? "configured " : " ",
		status & CG_NET_INTERFACE_CONNECTED ? "connected " : "disconnected ",
		status & CG_NET_INTERFACE_CAN_CONNECT ? "can-connect" : "");
}


int
main(void)
{
	cg_status_t cg_status;
	int32_t level;

	cg_init("example_net_connect");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	cg_net_register_internet_events(internet_status_callback);

	cg_net_register_interface_events (dev_name, interface_event_callback);

	cg_status = cg_net_connect();

	switch (cg_status) {
	case CG_STATUS_OK:
		printf("Peripheral connecting\n");
		break;
	case CG_STATUS_RESOURCE_BUSY:
		printf("Peripheral reset resource busy\n");
		break;
	case CG_STATUS_ERROR:
		printf("Peripheral reset error\n");
		break;
	default:
		printf("Peripheral reset unknown error\n");
		break;
	}

	printf ("Press any key to exit\n");
	getchar();
	tear_down();

	return 0;
}
