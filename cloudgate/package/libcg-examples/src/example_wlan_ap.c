/* EXAMPLES:
 *  all wlan_ap functions
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wlan.h>

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

int
main(int argc, char **argv)
{
	int32_t level;
	cg_status_t stat;

	char *old_ssid = NULL, *other_ssid = NULL;
	uint8_t old_broadcast, other_broadcast;
	uint16_t old_channel, auto_channel;
	cg_wlan_auth_params_t old_auth, uap1_auth;

	cg_init("WLAN examples");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	/* Reading all original settings, so we can easily restore them afterwards */
	stat = cg_wlan_ap_get_ssid("uap0", &old_ssid, &old_broadcast);
	if (stat == CG_STATUS_OK) {
		printf ("SSID for uap0: %s (broadcast: %d)\n", old_ssid, old_broadcast);
		// Freeing old_ssid at the end
	} else {
		printf ("Get ssid failed\n");
	}

	stat = cg_wlan_ap_get_ssid("unknown_dev", &other_ssid, &other_broadcast);
	if (stat == CG_STATUS_OK) {
		printf ("SSID for unknown device is: %s (broadcast: %d)\n", old_ssid, old_broadcast);
	} else {
		printf ("Get ssid for an unknown device failed: status %s\n", status_to_text(stat));
	}

	stat = cg_wlan_ap_get_channel("uap0", &old_channel);
	if (stat == CG_STATUS_OK) {
		printf ("Channel for uap0: %d\n", old_channel);
	} else {
		printf ("Get channel failed\n");
	}

	stat = cg_wlan_ap_get_auth_params("uap0", &old_auth);
	if (stat == CG_STATUS_OK) {
		printf ("Auth type for uap0: %d (password: \"%s\")\n", old_auth.type, old_auth.password);
	} else {
		printf ("Get auth failed\n");
	}

	stat = cg_wlan_ap_get_auth_params("uap1", &uap1_auth);
	if (stat == CG_STATUS_OK) {
		printf ("Auth type for uap1: %d (password: \"%s\")\n", uap1_auth.type, uap1_auth.password);
	} else {
		printf ("Get auth failed\n");
	}

	/* Setting values  */

	stat = cg_wlan_ap_set_ssid ("uap0", "my_test_ssid", 1);
	if (stat == CG_STATUS_OK) {
		printf ("Setting SSID was a success\n");
	} else {
		printf ("Setting SSID failed\n");
	}

	stat = cg_wlan_ap_set_channel ("uap0", 11);
	if (stat == CG_STATUS_OK) {
		printf ("Setting channel was a success\n");
	} else {
		printf ("Setting channel failed\n");
	}

	/* Automatic channel selection */
	stat = cg_wlan_ap_set_channel("uap0", 0);
	if (stat == CG_STATUS_OK) {
		printf("Setting automatic channel selection was a success\n");
	} else {
		printf("Setting automatich channel selection failed\n");
	}

	stat = cg_wlan_ap_get_channel("uap0", &auto_channel);
	if (stat == CG_STATUS_OK) {
		printf ("Channel for uap0: %d\n", auto_channel);
		printf("Automatic channel selection is %s\n", (0 == auto_channel ? "enabled" : "disabled"));
	} else {
		printf ("Get channel failed\n");
	}

	cg_wlan_auth_params_t new_auth;
	new_auth.type = CG_WLAN_AUTH_WPA2_PSK;
	strncpy(new_auth.password, "my_very_new_password", CG_PASSWORD_MAX_LENGTH);

	stat = cg_wlan_ap_set_auth_params ("uap0", &new_auth);
	if (stat == CG_STATUS_OK) {
		printf ("Setting authentication was a success\n");
	} else {
		printf ("Setting authentication failed\n");
	}

	char restore;
	printf("Do you want to restore original settings now? [y/n]: ");
	restore = getchar();
	if(restore == 'n' || restore == 'N')
		goto out;

	/* Setting original values  */
	stat = cg_wlan_ap_set_ssid ("uap0", old_ssid, old_broadcast);
	if (stat == CG_STATUS_OK) {
		printf ("Resetting SSID was a success\n");
	} else {
		printf ("Resetting SSID failed\n");
	}

	stat = cg_wlan_ap_set_channel ("uap0", old_channel);
	if (stat == CG_STATUS_OK) {
		printf ("Resetting channel was a success\n");
	} else {
		printf ("Resetting channel failed\n");
	}

	stat = cg_wlan_ap_set_auth_params ("uap0", &old_auth);
	if (stat == CG_STATUS_OK) {
		printf ("Resetting authentication was a success\n");
	} else {
		printf ("Resetting authentication failed\n");
	}

out:
	// Free all memory
	if(old_ssid)
		free(old_ssid);

	cg_deinit();

	return 0;
}
