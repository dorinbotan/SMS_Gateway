/* EXAMPLE:
 *  cg_get_serial_number
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_general.h>


int
main(void)
{
	cg_status_t cg_status;
	int32_t level;
	char *serial_number;

	cg_init("example_serial_number");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	cg_status = cg_get_serial_number(&serial_number);
	switch (cg_status) {
	case CG_STATUS_OK:
		if (strlen(serial_number) != 0) {
			printf("The serial number of the device is '%s'\n", serial_number);
		} else {
			printf("The serial number of the device is not set\n");
		}
		free(serial_number);
		break;
	case CG_STATUS_ERROR:
		printf("A generic error occurred when retrieving the serial number from the device\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("An invalid parameter is provided\n");
		break;
	default:
		printf("An unknown error occurred when retrieving the serial number from the device\n");
		break;
	}

	cg_deinit();

	return 0;
}
