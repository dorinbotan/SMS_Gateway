/* EXAMPLE:
 *  cg_system_log
 */

#include <stdio.h>

#include <unistd.h>

#include <libcg/cg_general.h>

int
main(void)
{
	cg_status_t cg_status;
	int32_t level;

	cg_init("example_log");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	cg_status = cg_system_log(CG_LOG_INFO, "api_level %d", level);
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("System logging OK\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("System logging invalid parameter\n");
		break;
	case CG_STATUS_ERROR:
		printf("System logging error\n");
		break;
	default:
		printf("System logging unknown error\n");
	}
	sleep(1);

	cg_status = cg_system_log(CG_LOG_DEBUG, "Example log level DEBUG 0x%04x",
								CG_LOG_DEBUG);
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("System logging OK\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("System logging invalid parameter\n");
		break;
	case CG_STATUS_ERROR:
		printf("System logging error\n");
		break;
	default:
		printf("System logging unknown error\n");
	}
	sleep(1);

	cg_status = cg_system_log(CG_LOG_INFO,  "Example log level INFO  0x%04x",
								CG_LOG_INFO);
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("System logging OK\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("System logging invalid parameter\n");
		break;
	case CG_STATUS_ERROR:
		printf("System logging error\n");
		break;
	default:
		printf("System logging unknown error\n");
	}
	sleep(1);

	cg_status = cg_system_log(CG_LOG_WARN,  "Example log level WARN  0x%04x",
								CG_LOG_WARN);
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("System logging OK\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("System logging invalid parameter\n");
		break;
	case CG_STATUS_ERROR:
		printf("System logging error\n");
		break;
	default:
		printf("System logging unknown error\n");
	}
	sleep(1);

	cg_status = cg_system_log(CG_LOG_ERR,   "Example log level ERR   0x%04x",
								CG_LOG_ERR);
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("System logging OK\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("System logging invalid parameter\n");
		break;
	case CG_STATUS_ERROR:
		printf("System logging error\n");
		break;
	default:
		printf("System logging unknown error\n");
	}
	sleep(1);

	cg_deinit();

	return 0;
}
