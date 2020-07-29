/* EXAMPLE:
 * cg_net_register_#X_events
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>
#include <libcg/cg_wwan.h>

char * dev_name = NULL;

static void
itf_status_callback (const char *dev_name, cg_net_if_status_t status)
{
	printf ("Interface event on %s %s%s%s%s\n", dev_name,
		status & CG_NET_INTERFACE_ENABLED ? "enabled " : " ",
		status & CG_NET_INTERFACE_CONFIGURED ? "configured " : " ",
		status & CG_NET_INTERFACE_CONNECTED ? "connected " : "disconnected ",
		status & CG_NET_INTERFACE_CAN_CONNECT ? "can-connect" : "");
}

static void
internet_status_callback (cg_net_internet_status_t status)
{
	printf ("Internet event %s%s%s%s\n",
		status == CG_NET_INTERNET_DISCONNECTED ? "disconnected " : " ",
		status == CG_NET_INTERNET_CONNECTING ? "connecting " : " ",
		status == CG_NET_INTERNET_CONNECTED ? "connected" : "",
		status == CG_NET_INTERNET_ON_DEMAND ? "on-demand" : "");
}

int
main(void)
{
	int32_t level;

	cg_init("example_net_register");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	printf("Listening...\n");

	cg_net_register_internet_events(internet_status_callback);

	cg_net_register_interface_events("eth0", itf_status_callback);
	cg_net_register_interface_events("3g", itf_status_callback);
	cg_net_register_interface_events(dev_name, itf_status_callback);

	printf ("Press any key to stop\n");
	getchar();

	cg_net_deregister_interface_events (dev_name);
	cg_net_deregister_interface_events ("3g");
	cg_net_deregister_interface_events ("eth0");

	cg_net_deregister_internet_events();

	cg_deinit();
	free (dev_name);
	return 0;
}
