/* EXAMPLE:
 * cg_wwan_signal_strength
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
	cg_status_t stat ;
	int8_t strength = 0, ecio = 0 ;
	char *dev_name = NULL;

	cg_init("example_wwan_signal_strength");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	stat = cg_wwan_signal_strength(dev_name, &strength, &ecio) ;
	if(stat == CG_STATUS_OK ) {
		printf("signal quality: %d, ecio quality: %d\n", strength, ecio) ;
	} else {
		printf("image type request failed with erc = %d\n", (int) stat) ;
	}
	free(dev_name);
	cg_deinit();

	return 0;
}
