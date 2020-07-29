/* EXAMPLE:
 *  cg_wwan_get_diagnostics
 */

#include <stdio.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>


int
main(void)
{
	cg_status_t cg_status;
	cg_wwan_diag_param_t *params;
	char *dev_name = NULL;
	uint32_t num_params;
	int32_t level;
	int i;

	cg_init("example_net_diagnostics");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	cg_status = cg_wwan_get_diagnostics(dev_name, &num_params, &params);
	switch (cg_status) {
	case CG_STATUS_OK:
		printf("Diagnostic parameters:\n");
		for (i = 0; i < num_params; i++) {
			printf("'%s'='%s'\n", params[i].name, params[i].value);
		}
		free(params);
		break;
	case CG_STATUS_ERROR:
		printf("A generic error occurred when retrieving WWAN diagnostics for '%s'\n", dev_name);
		break;
	case CG_STATUS_INVALID_PARAMETER:
		printf("An invalid parameter is provided\n");
		break;
	default:
		printf("An unknown error occurred when retrieving WWAN diagnostics for '%s'\n", dev_name);
		break;
	}
	free(dev_name);
	cg_deinit();
	return 0;
}
