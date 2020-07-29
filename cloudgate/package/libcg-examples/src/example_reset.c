/* EXAMPLE:
 *  cg_reset_system
 */

#include <stdio.h>

#include <libcg/cg_general.h>

int
main(void)
{
	cg_status_t cg_status;
	int32_t level;

	cg_init("example_reset");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	cg_status = cg_reset_peripherals();
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("Resetting peripherals\n");
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

	cg_status = cg_reset_system(FALSE, NULL);
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("Resetting system\n");
		break;
	case CG_STATUS_RESOURCE_BUSY:
		printf("Reset resource busy\n");
		break;
	case CG_STATUS_ERROR:
		printf("Reset error\n");
		break;
	default:
		printf("Reset unknown error\n");
		break;
	}

	cg_deinit();

	return 0;
}
