/* EXAMPLE:
 *  cg_net_datacounter_set_enabled
 *  cg_net_datacounter_get_enabled
 *  cg_net_datacounter_reset_stats
 *  cg_net_datacounter_get_stats
 */

#include <stdint.h>
#include <stdio.h>

#include <unistd.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>

int
main(void)
{
	cg_status_t cg_status;
	cg_net_stats_t stats;
	const char dev_name[] = "eth0";
	int32_t level;
	int enabled;

	cg_init("example_net_datacounter");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	cg_status = cg_net_datacounter_get_enabled(dev_name, &enabled);
	if (cg_status == CG_STATUS_OK) {
		printf("Datacounter state for device '%s': %d\n", dev_name, enabled);
	}

	enabled = TRUE;
	cg_net_datacounter_set_enabled(dev_name, enabled);

	cg_status = cg_net_datacounter_get_enabled(dev_name, &enabled);
	if (cg_status == CG_STATUS_OK) {
		printf("Datacounter state for device '%s': %d\n", dev_name, enabled);
	}

	cg_status = cg_net_datacounter_get_stats(dev_name, &stats);
	if (cg_status == CG_STATUS_OK) {
		printf("Datacounter stats for device '%s':\n", dev_name);
		printf("RX packets: %llu\n", stats.rx_packets);
		printf("RX bytes  : %llu\n", stats.rx_bytes);
		printf("TX packets: %llu\n", stats.tx_packets);
		printf("TX bytes  : %llu\n", stats.tx_bytes);
	}

	cg_net_datacounter_reset_stats(dev_name);
	sleep(5);

	cg_status = cg_net_datacounter_get_stats(dev_name, &stats);
	if (cg_status == CG_STATUS_OK) {
		printf("Datacounter stats for device '%s':\n", dev_name);
		printf("RX packets: %llu\n", stats.rx_packets);
		printf("RX bytes  : %llu\n", stats.rx_bytes);
		printf("TX packets: %llu\n", stats.tx_packets);
		printf("TX bytes  : %llu\n", stats.tx_bytes);
	}

	enabled = FALSE;
	cg_net_datacounter_set_enabled(dev_name, enabled);

	cg_status = cg_net_datacounter_get_enabled(dev_name, &enabled);
	if (cg_status == CG_STATUS_OK) {
		printf("Datacounter state for device '%s': %d\n", dev_name, enabled);
	}

	cg_status = cg_net_datacounter_get_stats(dev_name, &stats);
	if (cg_status == CG_STATUS_OK) {
		printf("Datacounter stats for device '%s':\n", dev_name);
		printf("RX packets: %llu\n", stats.rx_packets);
		printf("RX bytes  : %llu\n", stats.rx_bytes);
		printf("TX packets: %llu\n", stats.tx_packets);
		printf("TX bytes  : %llu\n", stats.tx_bytes);
	}

	cg_net_datacounter_reset_stats(dev_name);

	cg_status = cg_net_datacounter_get_stats(dev_name, &stats);
	if (cg_status == CG_STATUS_OK) {
		printf("Datacounter stats for device '%s':\n", dev_name);
		printf("RX packets: %llu\n", stats.rx_packets);
		printf("RX bytes  : %llu\n", stats.rx_bytes);
		printf("TX packets: %llu\n", stats.tx_packets);
		printf("TX bytes  : %llu\n", stats.tx_bytes);
	}

	cg_deinit();

	return 0;
}
