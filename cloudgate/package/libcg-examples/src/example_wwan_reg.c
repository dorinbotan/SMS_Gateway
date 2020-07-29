/* EXAMPLES:
 *  cg_wwan_*_reg
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>

static const char*
syst_to_str (cg_wwan_system_t sys)
{
	printf("syst_to_str system_type %08x\n", sys);
	if(sys & CG_SYS_LTE) return "lte";
	else if(sys & CG_SYS_HSPAPLUS) return "hspaplus";
	else if(sys & CG_SYS_HSDPAPLUS) return "hsdpaplus";
	else if(sys & CG_SYS_HSPA) return "hspa";
	else if(sys & CG_SYS_HSUPA) return  "hsupa";
	else if(sys & CG_SYS_HSDPA) return "hsdpa";
	else if(sys & CG_SYS_WCDMA) return "wcdma";
	else if(sys & CG_SYS_EVDOREVA) return "evdoreva";
	else if(sys & CG_SYS_EVDOREL0) return "evdere0";
	else if(sys & CG_SYS_EVDO) return "evdo";
	else if(sys & CG_SYS_CDMA) return "cdma";
	else if(sys & CG_SYS_EDGE) return "edge";
	else if(sys & CG_SYS_GPRS) return "gprs";
	else if(sys & CG_SYS_GSM) return "gsm";
	else if(sys & CG_SYS_NONE) return "none";
	return "unknown";
}

static const char*
creg_to_str (cg_wwan_creg_status_t status)
{
	switch (status) {
	case CG_WWAN_CREG_NOT_REG:
		return "Not registered, WWAN device is not currently searching a new operator to register to";
	case CG_WWAN_CREG_REG:
		return "Registered, home network";
	case CG_WWAN_CREG_SEARCHING:
		return "Not registered, but WWAN device is currently searching a new operator to register to";
	case CG_WWAN_CREG_DENIED:
		return "Registration denied";
	case CG_WWAN_CREG_ROAMING:
		return "Registered, roaming";
	}
	return "Unknown";
}

static void
reg_state_cb(const char *dev_name, cg_wwan_reg_state_t *state, void *context)
{
	printf ("reg_state_cb: name: %s, {cs: %s\n\tps: %s\n\tsys: %s\n}",
	        state->network_name,
	        creg_to_str (state->cs_state),
	        creg_to_str (state->ps_state),
	        syst_to_str (state->system_type));
}

static int
handle_stat (cg_status_t status, const char *str)
{
	switch(status) {
		case CG_STATUS_OK:
			printf("%s: Successful\n", str);
			break;

		case CG_STATUS_ERROR:
			printf("%s: An error occurred\n", str);
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("%s: Entered invalid parameter\n", str);
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("%s: Unknown error occurred\n", str);
			break;
	}

	return (status == CG_STATUS_OK);
}

static void
wwan_network_list_cb (cg_status_t status, cg_wwan_network_list_t *list, void *context)
{
	const char *dev_name = (const char *) context;
	cg_wwan_network_t *network = NULL;
	int i;
	cg_status_t stat;

	printf ("Found %d networks:\n", list->entries);
	for (i = 0; i < list->entries; i++) {
		network = &list->networks[i];

		printf ("\tNetwork: %s sys: %s id: %d, pref: %d, roaming: %d, forbidden: %d, home: %d\n",
		        network->name,
		        syst_to_str(network->system_type), network->id, network->preferred,
		        network->roaming, network->forbidden, network->home);
	}

	if (network != NULL) {
		printf ("Forcing manual network selection on last found network\n"
		        "cg_wwan_set_nw_selection_mode (%s, 0, %s)\n", dev_name, network->name);
		/* */
		stat = cg_wwan_set_nw_selection_mode (dev_name, 0, network);
		handle_stat (stat, "set new selection mode");
	}
}

int
main(int argc, char **argv)
{
	int32_t level;
	cg_status_t stat;
	char *dev_name = NULL;
	cg_wwan_network_t network;
	uint8_t is_auto;

	cg_init("WWAN examples");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	cg_wwan_reg_state_t state;

	stat = cg_wwan_register_reg_state_notification(dev_name, reg_state_cb, NULL);
	handle_stat (stat, "Register callback");

	stat = cg_wwan_get_nw_selection_mode (dev_name, &is_auto, &network);
	handle_stat (stat, "Get nw selection mode");
	printf ("Current nw selection mode: %s %s\n", is_auto ? "auto" : "manual", strlen(network.name) > 0 ? network.name : "none");

	stat = cg_wwan_get_reg_state (dev_name, &state);
	if (handle_stat  (stat, "get_reg_state")) {
		switch (state.cs_state) {
		case CG_WWAN_CREG_NOT_REG:
			printf ("%s\n", creg_to_str (state.cs_state));
			stat = cg_wwan_search_networks(dev_name, wwan_network_list_cb, (void*) dev_name);
			handle_stat (stat, "Search networks");
			break;
		case CG_WWAN_CREG_REG:
			printf ("%s\n", creg_to_str (state.cs_state));
			stat = cg_wwan_search_networks(dev_name, wwan_network_list_cb, (void*) dev_name);
			handle_stat (stat, "Search networks");
			break;
		case CG_WWAN_CREG_SEARCHING:
			printf ("%s\n", creg_to_str (state.cs_state));
			stat = cg_wwan_search_networks(dev_name, wwan_network_list_cb, (void*) dev_name);
			handle_stat (stat, "Search networks");
			break;
		case CG_WWAN_CREG_DENIED:
			printf ("%s\n", creg_to_str (state.cs_state));
			break;
		case CG_WWAN_CREG_ROAMING:
			printf ("%s\n", creg_to_str (state.cs_state));
			break;
		}
	}


	printf ("Press any key to exit\n");
	getchar();

	stat = cg_wwan_deregister_reg_state_notification (dev_name, reg_state_cb, NULL);
	handle_stat (stat, "Deregister callback");

	free (dev_name);
	cg_deinit();

	return 0;
}
