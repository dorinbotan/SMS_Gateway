/* EXAMPLES:
 *  all connection strategy functions
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>
#include <libcg/cg_wwan.h>

char * dev_name = NULL;

static inline char *status_to_text(int status_value)
{
	switch(status_value)
	{
		case CG_STATUS_OK:
			return "OK";

		case CG_STATUS_ERROR:
			return "ERROR";

		case CG_STATUS_RESOURCE_BUSY:
			return "RESOURCE BUSY";

		case CG_STATUS_INVALID_PARAMETER:
			return "INVALID_PARAMETER";

		case CG_STATUS_RESOURCE_UNAVAILABLE:
			return "RESOURCE_UNAVAILABLE";

		default:
			return "";
	}
}

static inline char *strategy_to_text(cg_net_conn_strat_t strategy)
{
	switch(strategy)
	{
		case CG_NET_STRATEGY_MANUAL:
			return "manual";

		case CG_NET_STRATEGY_PRIORITY:
			return "priority based";

		default:
			return "";
	}
}

int
main(int argc, char **argv)
{
	int32_t level;
	int i;
	cg_status_t stat;

	cg_net_conn_strat_t old_strategy;
	cg_dev_name_t *old_priority_list = NULL;
	char *old_manual_dev = NULL;
	uint32_t old_entries;

	cg_init("Connection strategy examples");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	/* Reading all original settings, so we can easily restore them afterwards */
	stat = cg_net_get_conn_strat(&old_strategy);
	if (stat == CG_STATUS_OK) {
		printf ("Current connection strategy: %s\n", strategy_to_text(old_strategy));
	}
	else {
		printf ("Get connection strategy failed, status: %s\n", status_to_text(stat));
		goto out;
	}

	if(old_strategy == CG_NET_STRATEGY_MANUAL) {
		stat = cg_net_get_manual_conn_device(&old_manual_dev); /* old_manual_dev is freed at the end of the program */
		if(stat == CG_STATUS_OK) {
			printf("Current manual connection device: %s\n", old_manual_dev);
		}
		else {
			printf("Get manual connection device failed, status: %s\n", status_to_text(stat));
			goto out;
		}
	}
	else {
		stat = cg_net_get_conn_priority_list(&old_entries, &old_priority_list); /* old_priority_list is freed at the end of the program */
		if(stat == CG_STATUS_OK) {
			printf("%d WAN interfaces found in the priority list\n", old_entries);
			for(i = 0; i < old_entries; i++) {
				printf("Interface %d: %s\n", i, old_priority_list[i]);
			}
		}
		else {
			printf("Get priority list failed, status: %s\n", status_to_text(stat));
			goto out;
		}
	}

	/* Setting values  */
	stat = cg_net_set_manual_conn_strat("mlan0");
	if(stat == CG_STATUS_OK) {
		printf("Setting the connection strategy to manual was succesful\n");
	}
	else {
		printf("Setting the connection strategy to manual failed, status: %s\n", status_to_text(stat));
	}

	int entries = 2;
	cg_dev_name_t priority_list[2];

	strncpy(priority_list[0], dev_name, CG_DEV_NAME_MAX);
	strncpy(priority_list[1], "mlan0", CG_DEV_NAME_MAX);
	stat = cg_net_set_priority_conn_strat(entries, priority_list);
	if(stat == CG_STATUS_OK) {
		printf("Setting the connection strategy to priority based was succesful\n");
	}
	else {
		printf("Setting the connection strategy to priority based failed, status: %s\n", status_to_text(stat));
	}

	char restore;
	printf("Do you want to restore the original settings? [y/n]: ");
	restore = getchar();
	if(restore == 'n' || restore == 'N') {
		goto out;
	}

	/* Setting original values  */
	if(old_strategy == CG_NET_STRATEGY_MANUAL) {
		stat = cg_net_set_manual_conn_strat(old_manual_dev);
		if(stat == CG_STATUS_OK) {
			printf("Resetting the manual connection strategy was succesful\n");
		}
		else {
			printf("Resetting the manual connection strategy failed, status: %s\n", status_to_text(stat));
		}
	}
	else {
		stat = cg_net_set_priority_conn_strat(old_entries, old_priority_list);
		if(stat == CG_STATUS_OK) {
			printf("Resetting the priority based connection strategy was succesful\n");
		}
		else {
			printf("Resetting the priority based connection strategy failed, status: %s\n", status_to_text(stat));
		}
	}

out:
	free(old_priority_list);
	free(old_manual_dev);

	cg_deinit();
	free (dev_name);
	return 0;
}
