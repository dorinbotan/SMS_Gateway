/* EXAMPLES:
 *  cg_wwan_*_reg
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>


static void
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
}

int
main(int argc, char **argv)
{
	int32_t level;
	cg_status_t stat;
	char *dev_name = NULL;
	cg_wwan_location_info_t *location_info=NULL;

	cg_init("WWAN location info example");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	stat = cg_wwan_get_location_info (dev_name, &location_info);
	handle_stat (stat, "Get location info");
	if (stat == CG_STATUS_OK) {
		if (location_info->lac) printf ("Location info: cell-id 0x%x location area code 0x%x\n", location_info->cell_id,location_info->lac);
		else if (location_info->tac) printf ("Location info: cell-id 0x%x tracking area code 0x%x\n", location_info->cell_id, location_info->tac);
	    else printf ("Location info: cell-id 0x%x\n", location_info->cell_id);
	}
	free(location_info);
	free (dev_name);
	cg_deinit();

	return 0;
}
