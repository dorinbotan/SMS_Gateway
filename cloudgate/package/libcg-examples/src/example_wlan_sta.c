/* EXAMPLE:
 *  cg_wlan_sta_scan_networks
 *  cg_wlan_sta_set_network
 *  cg_wlan_sta_get_connected_network
 *  cg_wlan_sta_get_network_list
 *  cg_wlan_sta_remove_network
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <semaphore.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wlan.h>


struct context {
	sem_t semaphore;
	uint32_t num_entries;
	cg_wlan_network_t *networks;
};


static const char *
auth_type_to_str(cg_wlan_auth_t auth_type)
{
	switch (auth_type) {
	case CG_WLAN_AUTH_OPEN:
		return "Open";
	case CG_WLAN_AUTH_WEP:
		return "WEP";
	case CG_WLAN_AUTH_WPA_PSK:
		return "WPA PSK";
	case CG_WLAN_AUTH_WPA2_PSK:
		return "WPA2 PSK";
	case CG_WLAN_AUTH_WPA_WPA2_PSK:
		return "WPA/WPA2 PSK";
	default:
		return "Unknown";
	}
}

void
wlan_network_list_cb(cg_status_t status, const char *dev_name,
                     uint32_t num_entries, cg_wlan_network_t *networks,
                     void *context)
{
	struct context *ctx = context;

	if (ctx != NULL) {
		ctx->num_entries = num_entries;
		ctx->networks = calloc(num_entries, sizeof(*ctx->networks));
		memcpy(ctx->networks, networks, num_entries * sizeof(*ctx->networks));
		sem_post(&ctx->semaphore);
	}
}

int
main(void)
{
	cg_status_t cg_status;
	cg_wlan_network_t network, *networks;
	struct context *ctx;
	const char dev_name[] = "mlan0";
	uint32_t num_entries;
	int32_t level;
	int i;

	cg_init("example_wlan_sta");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	ctx = calloc(1, sizeof(*ctx));
	sem_init(&ctx->semaphore, 0, 0);

	cg_status = cg_wlan_sta_scan_networks(dev_name, wlan_network_list_cb, ctx);
	if (cg_status == CG_STATUS_OK) {
		sem_wait(&ctx->semaphore);

		printf("Found %d network(s) on interface '%s'\n", ctx->num_entries, dev_name);
		for (i = 0; i < ctx->num_entries; i++) {
			cg_wlan_network_t *nw = &ctx->networks[i];

			printf("SSID: %32s, encryption: %12s, signal strength: %3d\n",
				nw->ssid, auth_type_to_str(nw->auth_type), nw->signal_strength);
		}
	}

	if (ctx->num_entries != 0) {
		cg_wlan_network_t *nw = &ctx->networks[0];
		cg_wlan_auth_params_t auth;

		auth.type = nw->auth_type;
		snprintf(auth.password, sizeof(auth.password), "password");

		cg_status = cg_wlan_sta_set_network(dev_name, nw->ssid, &auth);
		if (cg_status == CG_STATUS_OK) {
			printf("Added '%s' (%s) to the list of saved APs\n",
				nw->ssid, auth_type_to_str(nw->auth_type));
		}

		free(ctx->networks);
	}

	sem_destroy(&ctx->semaphore);
	free(ctx);

	/* The connection may not yet be established at this point in time.
	 * Refer to the documentation in <libcg/cg_net.h> for further details
	 * on following relevant functions:
	 *  - cg_net_register_interface_events()
	 *  - cg_net_deregister_interface_events()
	 *  - cg_net_register_internet_events()
	 *  - cg_net_deregister_internet_events()
	 *  - cg_net_get_internet_status()                                    */

	cg_status = cg_wlan_sta_get_connected_network(dev_name, &network);
	if (cg_status == CG_STATUS_OK) {
		if (network.ssid[0] != '\0') {
			printf("Connected to SSID '%s' (%s) on interface '%s'\n",
				network.ssid, auth_type_to_str(network.auth_type), dev_name);
			printf("Channel %d, signal strength %d\n", network.channel, network.signal_strength);
		}
	}

	cg_status = cg_wlan_sta_get_network_list(dev_name, &num_entries, &networks);
	if (cg_status == CG_STATUS_OK) {
		printf("%d network(s) in saved AP list for interface '%s'\n", num_entries, dev_name);
		for (i = 0; i < num_entries; i++) {
			cg_wlan_network_t *nw = &networks[i];

			printf("SSID: %32s, encryption: %12s\n",
				nw->ssid, auth_type_to_str(nw->auth_type));
		}
	}

	if (num_entries != 0) {
		cg_wlan_network_t *nw = &networks[0];

		cg_status = cg_wlan_sta_remove_network(dev_name, nw->ssid, nw->auth_type);
		if (cg_status == CG_STATUS_OK) {
			printf("Removed '%s' (%s) from the list of saved APs\n",
				nw->ssid, auth_type_to_str(nw->auth_type));
		}

		free(networks);
	}

	cg_deinit();

	return 0;
}
