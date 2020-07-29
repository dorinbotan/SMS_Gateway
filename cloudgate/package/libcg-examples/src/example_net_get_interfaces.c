/* EXAMPLE:
 * cg_net_get_interface_list
 * cg_net_get_interface_by_type
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_net.h>


int
main(void)
{
	int32_t level;
	uint32_t num, i, y;
	cg_net_if_t *interfaces = NULL;
	struct { int type; const char *friendly_name; } qry[5] = {
		{ CG_NET_TYPE_WWAN, "Mobile 3G data interface" },
		{ CG_NET_TYPE_NIC, "Regular ethernet interface" },
		{ CG_NET_TYPE_WLAN_AP, "Wireless LAN Access Point interface" },
		{ CG_NET_TYPE_WLAN, "Wireless LAN Station interface" },
		{ CG_NET_TYPE_BRIDGE, "Bridge interface" },
	};


	cg_init("example_net_get_interfaces");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	printf ("All interfaces:\n");

	cg_net_get_interface_list (&num, &interfaces);

	for (i=0; i<num; i++) {
		cg_net_if_t *iface = &interfaces[i];

		printf ("\t%s = %s%s%s\n", iface->dev_name,
		                       iface->status & CG_NET_INTERFACE_ENABLED    ? "enabled" : "disabled",
		                       iface->status & CG_NET_INTERFACE_CONFIGURED ? ",configured" : "",
		                       iface->status & CG_NET_INTERFACE_CONNECTED  ? ",connected" : "");
	}

	free (interfaces);
	interfaces = NULL;

	for (y=0; y<5; y++) {
		printf ("%s\n", qry[y].friendly_name);
		num = 0;
		interfaces = NULL;
		cg_net_get_interface_by_type (qry[y].type, &num, &interfaces);
		for (i=0; i<num; i++) {
			cg_net_if_t *iface = &interfaces[i];
			printf ("\t%s = %s\n", iface->dev_name,
			                       iface->status & CG_NET_INTERFACE_ENABLED ? "enabled" : "disabled");
		}
		free (interfaces);
	}

	cg_deinit();

	return 0;
}
