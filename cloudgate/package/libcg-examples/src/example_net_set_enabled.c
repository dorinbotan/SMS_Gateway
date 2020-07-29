/* EXAMPLE: cg_net_interface_set_enabled */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_net.h>
#include <libcg/cg_general.h>

int main(int argc, char **argv)
{
	cg_status_t status;
	const char *iface = argc == 2 ? argv[1] : "eth0";

	cg_init("example_net_set_enabled");

	/* Disable interface */
	status = cg_net_interface_set_enabled( iface, FALSE);

	switch(status) {
		case CG_STATUS_ERROR:
			printf("An error occured when disabling interface %s\n", iface);
			break;

		case CG_STATUS_OK:
			printf("Succesfully disabled interface %s\n", iface);
			break;
			
		case CG_STATUS_INVALID_PARAMETER:
		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Unknown error occured\n");
			break;
	}

	/* Sleep for a while */
	sleep(20);

	/* Re-enable interface */
	status = cg_net_interface_set_enabled( iface, TRUE);

	switch(status) {
		case CG_STATUS_ERROR:
			printf("An error occured when enabling interface %s\n", iface);
			break;

		case CG_STATUS_OK:
			printf("Succesfully re-enabled interface %s\n", iface);
			break;
			
		case CG_STATUS_INVALID_PARAMETER:
		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Unknown error occured\n");
			break;
	}

	cg_deinit();
	return 0;
}
