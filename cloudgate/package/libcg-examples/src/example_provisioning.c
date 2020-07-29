/* EXAMPLE:
 *  cg_prov_check_update
 *  cg_prov_get_status
 *  cg_get_slot_list
 */

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <libcg/cg_general.h>

int
main(void)
{
	cg_slot_list_t *slot;
	cg_status_t cg_status;
	cg_prov_status_t cg_prov_status;
	int32_t level;
	int i;

	cg_init("example_provisioning");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	cg_status = cg_get_slot_list(&slot);
	if (cg_status == CG_STATUS_OK) {
		for (i = 0; i < slot->num_slots; i++) {
			printf("Slot %d:\n", i);
			printf("\t%12s: %d\n", "type", slot->slots[i].type);
			printf("\t%12s: %d\n", "active", slot->slots[i].active);
			printf("\t%12s: %d\n", "size", slot->slots[i].size);
			printf("\t%12s: %s\n", "version", slot->slots[i].version);
			printf("\t%12s: %s\n", "uid", slot->slots[i].uid);
		}

		free(slot);
	}

	cg_status = cg_prov_check_update(FALSE);
	if (cg_status == CG_STATUS_OK) {
		do {
			sleep(1);
			cg_status = cg_prov_get_status(&cg_prov_status);
		} while ((cg_status == CG_STATUS_OK) &&
			 (cg_prov_status != CG_PROV_ERROR) &&
			 (cg_prov_status != CG_PROV_UNREGISTERED) &&
			 (cg_prov_status != CG_PROV_OK));

	} else {
		printf("Check for provisioning update failed\n");
	}

	cg_status = cg_get_slot_list(&slot);
	if (cg_status == CG_STATUS_OK) {
		for (i = 0; i < slot->num_slots; i++) {
			printf("Slot %d:\n", i);
			printf("\t%12s: %d\n", "type", slot->slots[i].type);
			printf("\t%12s: %d\n", "active", slot->slots[i].active);
			printf("\t%12s: %d\n", "size", slot->slots[i].size);
			printf("\t%12s: %s\n", "version", slot->slots[i].version);
			printf("\t%12s: %s\n", "uid", slot->slots[i].uid);
		}

		free(slot);
	}

	cg_deinit();

	return 0;
}
