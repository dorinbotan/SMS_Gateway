/* EXAMPLE:
 * cg_net_get_interfaces
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>

const char *mode_descr[] = { "CDMA", "UMTS" } ;

int
main(void)
{
	int32_t level;
	cg_wwan_mode_t mode;
	cg_status_t stat ;
	char *dev_name = NULL;

	cg_init("example_net_connect");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	stat = cg_wwan_get_mode(dev_name, &mode);
	if(stat == CG_STATUS_OK ) {
		printf("modem image type: %s\n", mode_descr[(int) mode]) ;
	} else {
		printf("image type request failed\n") ;
	}
	free (dev_name);
	cg_deinit();

	return 0;
}
