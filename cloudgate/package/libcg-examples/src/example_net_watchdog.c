/* EXAMPLE: cg_net_get_watchdog && cg_net_set_watchdog*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_net.h>
#include <libcg/cg_general.h>

int main(void)
{
	cg_net_watchdog_settings_t *wd_settings = NULL, settings;
	cg_status_t status;
	int i;

	cg_init("example_net_watchdog");

	/* Get the current watchdog settings */

	status = cg_net_get_watchdog(&wd_settings);

	switch(status) {
		case CG_STATUS_ERROR:
			printf("An error occured when retrieving the watchdog settings\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Get watchdog: Invalid parameter\n");
			break;

		case CG_STATUS_OK:
			printf("Watchdog settings:\nEnabled= %d\nUse_ping= %d\nInterval= %d\nAction= %d\n", \
			       wd_settings->enabled, wd_settings->use_ping, wd_settings->interval, wd_settings->action);
			for(i = 0; i < wd_settings->num_addresses; i++)
				printf("Address %d: %s\n", i, wd_settings->addresses[i]);

			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Unknown error occured\n");
			break;
	}
	free(wd_settings);

	/* Set new watchdog settings */

	settings.enabled       = TRUE;
	settings.use_ping      = TRUE;
	settings.interval      = 60;
	settings.action        = CG_CONN_WD_ACTION_RESET;

	memset(settings.addresses, 0, CG_MAX_PING_ADDRESSES * CG_MAX_ADDRESS_LENGTH);
	snprintf(settings.addresses[0], CG_MAX_ADDRESS_LENGTH,"192.168.1.5");
	snprintf(settings.addresses[1], CG_MAX_ADDRESS_LENGTH,"3ffe:6a88:85a3::1319:8a2e:0370:7344");

	status = cg_net_set_watchdog(&settings);

	switch(status) {
		case CG_STATUS_ERROR:
			printf("An error occured when setting the watchdog settings\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Set watchdog: Invalid parameter\n");
			break;

		case CG_STATUS_OK:
			printf("New watchdog settings applied\n");
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Unknown error occured\n");
			break;
	}

	cg_deinit();
	return 0;
}
