/* EXAMPLE: cg_device_list */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libcg/cg_general.h>
#include <libcg/cg_device.h>

int main(void)
{
	cg_status_t stat;

	uint32_t num_devices;
	cg_device_t* devices = NULL;
	int32_t level;

	cg_init("example_device_list");
	printf("cg_device_list\n");
	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	stat = cg_device_list(CG_DEVTYPE_NETWORK | CG_DEVTYPE_SERIAL, &num_devices, &devices);
	switch (stat)
	{
		case CG_STATUS_OK:                printf("request OK\n"); break;
		case CG_STATUS_ERROR:             printf("Device error\n"); break;
		case CG_STATUS_RESOURCE_BUSY:     printf("Device busy\n"); break;
		case CG_STATUS_INVALID_PARAMETER: printf("Device invalid parameter\n"); break;
		default:                          printf("Device unknown error\n"); break;
	}

	if (stat == CG_STATUS_OK) {
		int i;
		cg_device_t* dev = devices;
		printf("num: %d\n",num_devices);
		for(i=0;i<num_devices;i++,dev++)
			printf("dev: %s, path: %s, type: %02X, location: %02X, state: %02X\n", \
					dev->friendly_name,dev->device_name,dev->device_type,dev->device_location,dev->status);
	}

	if(devices)
		free(devices);
	devices = NULL;

	cg_deinit();
	return 0;
}
