/* EXAMPLE:
 *  cg_set_dev_mode
 */

#include <stdio.h>
#include <unistd.h>
#include <libcg/cg_general.h>

int
main(void)
{
	cg_status_t cg_status;
	int32_t level;

	cg_init("example_set_dev_mode");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	//http://www.google.com/search?q=choosing+good+passwords&oq=choosing+good+passwords
	cg_status = cg_set_dev_mode(TRUE ,CG_DEV_ROOT_ACCESS, "monkey");
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("You have now root shell access until you disable it again or until next reboot\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("One of your parameters is invalid!\n");
		break;
	case CG_STATUS_ERROR:
		printf("General Error!\n");
		break;
	default:
		printf("Unknown error\n");
	}

	cg_deinit();

	return 0;
}
