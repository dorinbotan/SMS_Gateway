/* EXAMPLE: cg_device_claim && cg_device_release */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libcg/cg_general.h>
#include <libcg/cg_device.h>
#include <libcg/cg_wwan.h>

char *dev_name = NULL;

cg_status_t device_claim_cb(cg_device_t *device, void *context)
{
	cg_status_t stat = CG_STATUS_ERROR;
	cg_device_t *dev = (cg_device_t *) context;
	printf("device_claim_cb: we keep device %s claimed\n", (dev!=NULL?dev->device_name:"unknown"));
	return stat;
}

int main(void)
{
	cg_status_t stat;
	cg_device_t gobi_dev = { "gobinet", "/dev/usb0", CG_DEVTYPE_NETWORK, 0};
	cg_device_t hpex_dev = { "hpexnet", "/dev/hso0", CG_DEVTYPE_NETWORK, 0};
	cg_device_t lte_dev = { "telinet", "/dev/wwan0", CG_DEVTYPE_NETWORK, 0};
	cg_device_t *dev;
	int32_t level;

	cg_init("example_device_claim");
	printf("cg_device_claim\n");
	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	if (strcmp(dev_name,"hso0")==0) {
		dev = &hpex_dev;
	}
	else if (strcmp(dev_name,"usb0")==0) {
		dev = &gobi_dev;
	}
	else {
		dev = &lte_dev;
	}
	stat = cg_device_claim(dev, device_claim_cb, dev);

	switch (stat)
	{
		case CG_STATUS_OK:                printf("Device claimed\n"); break;
		case CG_STATUS_ERROR:             printf("Device error\n"); break;
		case CG_STATUS_RESOURCE_BUSY:     printf("Device busy\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("Device invalid paramter\n"); break;
		default:                          printf("Device unknown error\n"); break;
	}

	sleep(100);

	if (stat == CG_STATUS_OK) {
		printf("cg_device_release\n");
		stat = cg_device_release (dev);
		switch (stat)
		{
			case CG_STATUS_OK:                printf("Device released\n"); break;
			case CG_STATUS_ERROR:             printf("Device error\n"); break;
			case CG_STATUS_RESOURCE_BUSY:     printf("Device busy\n"); break;
			case CG_STATUS_INVALID_PARAMETER: printf("Device invalid paramter\n"); break;
			default:                          printf("Device unknown error\n"); break;
		}
	}
	cg_deinit();
	free(dev_name);
	return 0;
}
