/* EXAMPLE:
 *  cg_net_set_config
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>
#include <libcg/cg_wwan.h>

char *dev_name = NULL;

int
main(void)
{
	cg_status_t cg_status;
	cg_net_cfg_t cfg;
	int32_t level;

	cg_init("example_net_lan_config");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	/* Configuration of a LAN interface */
	memset(&cfg, 0, sizeof(cfg));
	cfg.zone = CG_NET_ZONE_LAN;

	inet_pton(AF_INET, "192.168.1.1", &cfg.ip_config.ip_addr.ip.ipv4);
	inet_pton(AF_INET, "255.255.0.0", &cfg.ip_config.netmask.ip.ipv4);

	cfg.conf.lan.dhcp_config.enabled = 1;
	inet_pton(AF_INET, "192.168.1.100", &cfg.conf.lan.dhcp_config.ip_start.ip.ipv4);
	inet_pton(AF_INET, "192.168.1.150", &cfg.conf.lan.dhcp_config.ip_end.ip.ipv4);
	inet_pton(AF_INET, "192.168.1.21", &cfg.conf.lan.dhcp_config.dns1.ip.ipv4);
	inet_pton(AF_INET, "192.168.1.22", &cfg.conf.lan.dhcp_config.dns2.ip.ipv4);
	cfg.conf.lan.dhcp_config.leasetime.amount = 14;
	cfg.conf.lan.dhcp_config.leasetime.unit = CG_NET_UNIT_MINUTE;
	cfg.conf.lan.bridge_dev_name[0] = '\0';

	cg_status = cg_net_set_config("eth0", &cfg);
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("LAN configuration is successfully applied.\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("An invalid parameter is provided.\n");
		break;
	case CG_STATUS_ERROR:
		printf("Generic error occurred when applying new LAN configuration.\n");
		break;
	default:
		printf("Unknown error occurred when applying new LAN configuration.\n");
		break;
	}

	/* Configuration of a WAN interface */
	memset(&cfg, 0, sizeof(cfg));
	cfg.zone = CG_NET_ZONE_WAN;

	cfg.conf.wan.mode = CG_NET_MODE_ALWAYS_ON;
	cfg.conf.wan.ip_config_type = CG_DHCP;

	cg_status = cg_net_set_config(dev_name, &cfg);
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("WAN configuration is successfully applied.\n");
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("An invalid parameter is provided.\n");
		break;
	case CG_STATUS_ERROR:
		printf("Generic error occurred when applying new WAN configuration.\n");
		break;
	default:
		printf("Unknown error occurred when applying new WAN configuration.\n");
		break;
	}

	cg_deinit();
	free(dev_name);
	return 0;
}
