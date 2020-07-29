/* EXAMPLE:
 *  cg_net_register_interface_events, cg_net_register_internet_events
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>
#include <libcg/cg_wwan.h>

char * dev_name = NULL;

static void
internet_status_callback (cg_net_internet_status_t status)
{
	switch (status) {
	case CG_NET_INTERNET_DISCONNECTED:
		printf ("Internet event: CG_NET_INTERNET_DISCONNECTED\n");
		break;
	case CG_NET_INTERNET_CONNECTING:
		printf ("Internet event: CG_NET_INTERNET_CONNECTING\n");
		break;
	case CG_NET_INTERNET_CONNECTED:
		printf ("Internet event: CG_NET_INTERNET_CONNECTED\n");
		break;
	case CG_NET_INTERNET_ON_DEMAND:
		printf ("Internet event: CG_NET_INTERNET_ON_DEMAND\n");
		break;
	}
}

static void interface_event_callback (const char *dev_name, cg_net_if_status_t status)
{
	printf ("Interface event on %s %s%s%s%s\n", dev_name,
		status & CG_NET_INTERFACE_ENABLED ? "ENABLED " : " ",
		status & CG_NET_INTERFACE_CONFIGURED ? "CONFIGURED " : " ",
		status & CG_NET_INTERFACE_CONNECTED ? "CONNECTED " : " ",
		status & CG_NET_INTERFACE_CAN_CONNECT ? "CAN_CONNECT" : "");
}


int
main(int argc, char **argv)
{
	int32_t level;

	cg_init("sdk-events-viewer");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	cg_net_register_internet_events (internet_status_callback);
	cg_net_register_interface_events (dev_name, interface_event_callback);

	printf ("Awaiting internet and interface events on %s\n", dev_name);
	printf ("Press any key to exit\n");
	getchar();

	cg_net_deregister_interface_events (dev_name);
	cg_net_deregister_internet_events ();
	cg_deinit();
	free (dev_name);
	return 0;
}
