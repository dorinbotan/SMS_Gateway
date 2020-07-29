/* EXAMPLES:
 *  cg_wwan_get_activation_state, cg_wwan_get_meid, cg_wwan_activate
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>

int main(int argc, char **argv)
{
	int32_t level;
	cg_status_t stat;
	cg_wwan_act_state_t state = CG_WWAN_ACT_STATUS_NOT_ACTIVATED;
	char *meid;
	char *dev_name = NULL;

	cg_init("WWAN CDMA examples");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	stat = cg_wwan_get_meid(dev_name, &meid);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Get meid successful\nmeid: %s\n", meid);
			free(meid);
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while getting meid\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Get meid: Entered invalid parameter\n");
			break;

		default:
			printf("Get meid: Unknown error occurred\n");
			break;
	}

	stat = cg_wwan_get_activation_state(dev_name, &state);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Get activation state successful\nstate: %d\n", state);
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while getting activation state\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Get activation state: Entered invalid parameter\n");
			break;

		default:
			printf("Get activation state: Unknown error occurred\n");
			break;
	}

	if (state == CG_WWAN_ACT_STATUS_NOT_ACTIVATED) {
		stat = cg_wwan_activate(dev_name, "__CDMA_activation_code__");
		switch (stat) {
			case CG_STATUS_OK:
				printf("CDMA activation successful\n");
				break;

			case CG_STATUS_ERROR:
				printf("CDMA activation: Generic error occurred\n");
				break;

			case CG_STATUS_INVALID_PARAMETER:
				printf("CDMA activation: Entered invalid parameter\n");
				break;

			default:
				printf("CDMA activation: Unknown error occurred\n");
				break;
		}
	}

	free(dev_name);
	cg_deinit();
	return 0;

}
