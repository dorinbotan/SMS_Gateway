/* EXAMPLES:
 *  cg_wwan_get_phone_number, ...
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>

int main(int argc, char **argv)
{
	int32_t level;
	char *phone_number;
	cg_status_t status;
	char *dev_name;

	cg_init("WWAN examples");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev(&dev_name) != CG_STATUS_OK) {
		printf("Could not retrieve the active primary wwan device\n");
		cg_deinit();
		return 1;
	}

	printf("Active WWAN device: %s\n", dev_name);

	status = cg_wwan_get_phone_number(dev_name, &phone_number);

	switch (status) {
		case CG_STATUS_ERROR:
			printf("An error occured while retrieving the phone number\n");
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("An invalid parameter was provided to the function\n");
			break;
		case CG_STATUS_OK:
			printf("Successfully retrieved the phone number\n");

			if (!strcmp(phone_number, "unknown")) {
				printf("The phone number is not present on the SIM.\n");
			} else {
				printf("The phone number is %s\n", phone_number);
			}

			free(phone_number);
			break;
		default:
			printf("An unexpected error occured: %d\n", status);
	}

	free(dev_name);
	cg_deinit();

	return 0;
}
