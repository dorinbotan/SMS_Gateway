/* EXAMPLE:
 *  cg_net_disconnect
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>


static void
tear_down (void)
{
	cg_net_deregister_internet_events ();
	cg_deinit();
}

static void
internet_status_callback (cg_net_internet_status_t status)
{
	switch (status) {
	case CG_NET_INTERNET_DISCONNECTED:
		printf("Peripheral disconnected\n");

		tear_down();

		exit(0);

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

int
main(void)
{
	cg_status_t cg_status;
	int32_t level;

	cg_init("example_net_connect");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	cg_net_register_internet_events(internet_status_callback);

	cg_status = cg_net_disconnect();

	switch (cg_status) {
	case CG_STATUS_OK:
		printf("Peripheral disconnecting\n");
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
