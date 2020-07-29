/* EXAMPLE:
 *  cg_set_dev_mode
 */

#include <stdio.h>
#include <unistd.h>
#include <libcg/cg_general.h>
#include <libcg/cg_hal.h>

void set_uart4(const cg_hal_serial_cfg_t *config)
{
	cg_status_t cg_status;
	int reboot;

	cg_status = cg_hal_serial_set_config("/dev/ttySP4", config, &reboot);

	switch (cg_status) {
		case CG_STATUS_OK:
			if (reboot) {
				printf("UART settings stored, a reboot is required to make them work\n");
			}
			else {
				printf("UART settings stored\n");
			}
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
}

int
main(void)
{
	int32_t level;
	char selection;

	cg_hal_serial_cfg_t config_rs232      = { .serial_mode = CG_HAL_SERIAL_RS232 };
	cg_hal_serial_cfg_t config_rs485      = { .serial_mode = CG_HAL_SERIAL_RS485, .param.rs485.enable_termination = 0 };
	cg_hal_serial_cfg_t config_rs485_term = { .serial_mode = CG_HAL_SERIAL_RS485, .param.rs485.enable_termination = 1 };


	cg_init("example_set_dev_mode");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	printf("Make a selection by pressing number 0..4\n");
	printf("1. Set uart4 to rs232\n");
	printf("2. Set uart4 to rs485, no termination\n");
	printf("3. Set uart4 to rs485, termination enabled\n");
	printf("0. exit\n");
	selection = getchar();

	switch (selection)
	{
		case '0': break;
		case '1': set_uart4(&config_rs232);       break;
		case '2': set_uart4(&config_rs485);       break;
		case '3': set_uart4(&config_rs485_term);  break;
		default: printf("unknown key pressed\n"); break;
	}

	cg_deinit();
	return 0;
}
