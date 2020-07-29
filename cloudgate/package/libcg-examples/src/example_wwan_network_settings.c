/* EXAMPLES:
 *  cg_wwan_get_network_settings, cg_wwan_set_network_settings
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>

const char* cg_wwan_auth_type_to_string(cg_wwan_auth_type_t wwan_auth_type)
{
	switch(wwan_auth_type) {
		case CG_WWAN_AUTH_TYPE_NONE:
			return "CG_WWAN_AUTH_TYPE_NONE";
		case CG_WWAN_AUTH_TYPE_PAP:
			return "CG_WWAN_AUTH_TYPE_PAP";
		case CG_WWAN_AUTH_TYPE_CHAP:
			return "CG_WWAN_AUTH_TYPE_CHAP";
		case CG_WWAN_AUTH_TYPE_AUTO:
			return "CG_WWAN_AUTH_TYPE_AUTO";
	}

	return NULL;
}

void get_network_settings(char *dev_name)
{
	cg_wwan_network_settings_t *wwan_network_settings;
	cg_status_t stat;

	if (!dev_name) {
		printf("dev_name is NULL\n");
		return;
	}

	stat = cg_wwan_get_network_settings(dev_name, &wwan_network_settings);

	switch(stat) {
		case CG_STATUS_OK:
			printf("Network settings successfully retrieved\n");
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("An invalid parameter was provided when getting the network settings\n");
			break;
		case CG_STATUS_ERROR:
			printf("An error occured when getting the network settings\n");
			break;
		case CG_STATUS_RESOURCE_BUSY:
			printf("Device %s is busy. Cannot get the network settings\n", dev_name);
			break;
		case CG_STATUS_RESOURCE_UNAVAILABLE:
			printf("Device %s is currently unavailabe. Cannot get the network settings\n",
					dev_name);
			break;
		default:
			printf("Unknown error\n");
			break;
	}

	if (stat == CG_STATUS_OK) {
		printf("APN: %s\n", wwan_network_settings->apn);
		printf("Username: %s\n", wwan_network_settings->username);
		//Password is readonly and is always empty when retrieving the network settings.
		printf("Password: %s\n", wwan_network_settings->password);
		printf("Authentication type: %s\n", cg_wwan_auth_type_to_string(wwan_network_settings->auth_type));

		free(wwan_network_settings);
	}
}

void set_network_settings(char *dev_name)
{
	cg_wwan_network_settings_t wwan_network_settings;
	cg_status_t stat;

	if (!dev_name) {
		printf("dev_name is NULL\n");
		return;
	}

	memset(&wwan_network_settings, 0, sizeof(wwan_network_settings));

	strncpy(wwan_network_settings.apn, "internet.proximus.be", sizeof(wwan_network_settings.apn) - 1);
	strncpy(wwan_network_settings.username, "username", sizeof(wwan_network_settings.username) - 1);
	strncpy(wwan_network_settings.password, "password", sizeof(wwan_network_settings.password) - 1);
	wwan_network_settings.auth_type = CG_WWAN_AUTH_TYPE_AUTO;

	stat = cg_wwan_set_network_settings(dev_name, &wwan_network_settings);

	switch(stat) {
		case CG_STATUS_OK:
			printf("Network settings successfully set\n");
			break;
		case CG_STATUS_INVALID_PARAMETER:
			printf("An invalid parameter was provided when setting the network settings\n");
			break;
		case CG_STATUS_ERROR:
			printf("An error occured when setting the network settings\n");
			break;
		case CG_STATUS_RESOURCE_BUSY:
			printf("Device %s is busy. Cannot set the network settings\n", dev_name);
			break;
		case CG_STATUS_RESOURCE_UNAVAILABLE:
			printf("Device %s is currently unavailable. Cannot set the network settings\n",
					dev_name);
		default:
			printf("Unknown error\n");
			break;
	}
}

int
main(int argc, char **argv)
{
	int32_t level;
	char *dev_name = NULL;

	cg_init("WWAN examples");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	get_network_settings(dev_name);
	set_network_settings(dev_name);

	free(dev_name);
	cg_deinit();

	return 0;
}
