/* EXAMPLE:
 *  cg_net_set_config
 */

#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>


int
main(void)
{
	cg_status_t cg_status;
	cg_net_cfg_t cfg;
	int32_t level;

	cg_init("example_net_lan_config");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	/* Configuration of a LAN interface */

	memset(&cfg, 0, sizeof(cfg));
	cfg.zone = CG_NET_ZONE_LAN;
	inet_pton(AF_INET, "10.0.1.1", &cfg.ip_config.ip_addr.ip.ipv4);
	inet_pton(AF_INET, "255.255.255.0", &cfg.ip_config.netmask.ip.ipv4);
	cfg.conf.lan.bridge_dev_name[0] = '\0';
	cg_status = cg_net_set_config("uap0", &cfg);

	switch (cg_status) {
	case CG_STATUS_OK:
		printf("LAN configuration on uap0 is successfully applied.\n");
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

	memset(&cfg, 0, sizeof(cfg));
	cfg.zone = CG_NET_ZONE_LAN;
	inet_pton(AF_INET, "10.0.0.1", &cfg.ip_config.ip_addr.ip.ipv4);
	inet_pton(AF_INET, "255.255.255.0", &cfg.ip_config.netmask.ip.ipv4);
	cfg.conf.lan.bridge_dev_name[0] = '\0';
	cg_status = cg_net_set_config("uap1", &cfg);

	switch (cg_status) {
	case CG_STATUS_OK:
		printf("LAN configuration on uap1 is successfully applied.\n");
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

	cg_deinit();

	return 0;
}
