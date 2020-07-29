/* EXAMPLES:
 *  cg_wwan_get_serial, ...
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>

static void reg_state_cb(const char *dev_name, cg_wwan_reg_state_t *state, void *context) { }

int
main(int argc, char **argv)
{
	int32_t level, i, enabled;
	char *param = NULL;
	cg_wwan_img_list_t *list = NULL;
	cg_status_t stat;
	cg_wwan_reg_state_t reg_state;
	char *dev_name = NULL;

	cg_init("WWAN examples");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	stat = cg_wwan_get_serial (dev_name, &param);
	if (stat == CG_STATUS_OK) {
		printf ("Serial: %s\n", param);
		free (param); param = NULL;
	} else {
		printf ("get serial failed\n");
	}

	stat = cg_wwan_get_imei (dev_name, &param);
	if (stat == CG_STATUS_OK) {
		printf ("IMEI: %s\n", param);
		free (param); param = NULL;
	} else {
		printf ("get imei failed\n");
	}

	stat = cg_wwan_get_imsi (dev_name, &param);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Get imsi successful\n%s\n", param);
			free (param); param = NULL;
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while getting imsi\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Get imsi: Entered invalid parameter\n");
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Get imsi: Unknown error occurred\n");
			break;
	}

	stat = cg_wwan_gobi_get_image_type (dev_name, argc > 1 ? argv[1] : "UMTS Generic", &param);
	if (stat == CG_STATUS_OK) {
		printf ("Image type: %s\n", param);
		free (param); param = NULL;
	} else {
		printf ("get image type failed\n");
	}

	stat = cg_wwan_get_iccid (dev_name, &param);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Get iccid successful\n%s\n", param);
			free (param); param = NULL;
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while getting iccid\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Get iccid: Entered invalid parameter\n");
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Get iccid: Unknown error occurred\n");
			break;
	}

	stat = cg_wwan_gobi_set_active_image (dev_name, argc > 1 ? argv[1] : "UMTS Generic");

	if (stat != CG_STATUS_OK) {
		printf ("set active image failed\n");
	}

	// Wait until the modem is initialized again.
	sleep(40);

	stat = cg_wwan_gobi_get_active_image (dev_name, &param);

	if (stat == CG_STATUS_OK) {
		printf ("Active image: %s\n", param);
		free(param); param = NULL;
	} else {
		printf ("get active image failed\n");
	}

	stat = cg_wwan_gobi_get_image_list (dev_name, &list);

	if (stat == CG_STATUS_OK) {
		printf ("Available images (current=%d):\n", list->current);
		for (i=0; i<list->entries; i++) {
			printf ("\t%s\n",list->images[i].name);
		}
		free (list);
	} else {
		printf ("get image list failed\n");
	}

	stat = cg_wwan_register_reg_state_notification(dev_name, reg_state_cb, NULL);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Successfully registered callback\n");
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while registering callback\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Register callback: Entered invalid parameter\n");
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Register callback: Unknown error occurred\n");
			break;
	}

	stat = cg_wwan_deregister_reg_state_notification(dev_name, reg_state_cb, NULL);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Successfully deregistered callback\n");
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while deregistering callback\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Deregister callback: Entered invalid parameter\n");
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Deregister callback: Unknown error occurred\n");
			break;
	}

	stat = cg_wwan_get_reg_state(dev_name, &reg_state);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Get reg state successful\ncs_state: %d\nps_state: %d\nsystem_type: %d\nnetwork_name: %s\n",
			       reg_state.cs_state, reg_state.ps_state, reg_state.system_type, reg_state.network_name);
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while getting reg state\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Get reg state: Entered invalid parameter\n");
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Get reg state: Unknown error occurred\n");
			break;
	}

	stat = cg_wwan_get_radio(dev_name, &enabled);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Get radio successful\nRadio state: %s\n", enabled != FALSE ? "enabled" : "disabled");
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while getting radio\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Get radio: Entered invalid parameter\n");
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Get radio: Unknown error occurred\n");
			break;
	}

	enabled = TRUE;
	stat = cg_wwan_set_radio(dev_name, enabled);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Set radio successful\nRadio set to state: %s\n", enabled != FALSE ? "enabled" : "disabled");
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while setting radio\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Set radio: Entered invalid parameter\n");
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Set radio: Unknown error occurred\n");
			break;
	}

	enabled = TRUE;
	stat = cg_wwan_set_connection_hunting(dev_name, enabled, 10);
	switch(stat) {
		case CG_STATUS_OK:
			printf("Set connection hunting successful\nConnection hunting set to state: %s\n", enabled != FALSE ? "enabled" : "disabled");
			break;

		case CG_STATUS_ERROR:
			printf("An error occurred while setting connection hunting\n");
			break;

		case CG_STATUS_INVALID_PARAMETER:
			printf("Set connection hunting: Entered invalid parameter\n");
			break;

		case CG_STATUS_RESOURCE_BUSY:
		default:
			printf("Set connection hunting: Unknown error occurred\n");
			break;
	}

	free(dev_name);
	cg_deinit();

	return 0;
}
