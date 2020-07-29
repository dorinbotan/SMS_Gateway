/* EXAMPLE:
 *  cg_net_get_disabled_at_boot
 *  cg_net_set_disabled_at_boot
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>

int
main(void)
{
	cg_status_t cg_status;
	int32_t level, disabled;
	#define NUM_DEVICES 7
	const char devices[NUM_DEVICES][8] = {"eth0", "eth1", "uap0", "uap1", "usb0", "usb1", "wwan0" };
	int32_t input, i;
	char buffer[32];

	cg_init("example_net_disabled_at_boot");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	for ( i=0 ; i<NUM_DEVICES ; ++i) {
		cg_status = cg_net_get_disabled_at_boot(devices[i],&disabled);
		printf("%02d) %10s: %s\n", i+1, devices[i], ( cg_status == CG_STATUS_OK ? ( disabled?"disabled":"enabled") : "error"));
	}

	printf("enter number to toggle setting or any char to exit\n");
	input = atoi(fgets(buffer,sizeof(buffer), stdin));

	if ( (input>0) && (input <= NUM_DEVICES)) {
		cg_net_get_disabled_at_boot(devices[input-1],&disabled);
		disabled = !disabled;
		cg_net_set_disabled_at_boot(devices[input-1],disabled);

		switch (cg_status) {
		case CG_STATUS_OK:
			printf("Status changed\n");
			break;
		case CG_STATUS_RESOURCE_BUSY:
			printf("resource busy\n");
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("parameter invalid\n");
			break;
		case CG_STATUS_ERROR:
			printf("reset error\n");
			break;
		default:
			printf("unknown error\n");
			break;
		}
	}
	cg_deinit();

	return 0;
}
