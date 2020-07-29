/* EXAMPLE: cg_sms_set_smsc & cg_sms_get_smsc */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>
#include <libcg/cg_sms.h>

#define SMS_SERVICE_CENTER "+32475161616"
char * dev_name = NULL;


int main()
{
	cg_status_t stat;
	int32_t level;
	char *number = NULL;
	int exitcode = -1;

	cg_init("SMS example");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		goto exit;
	}
	printf ("Active primary WWAN device: %s\n",dev_name);

	stat =  cg_sms_set_smsc(dev_name, SMS_SERVICE_CENTER);

	if(stat != CG_STATUS_OK) {
		fprintf(stderr, "Can't set sms service center (%d)\n",stat);
		goto exit;
	}

	stat =  cg_sms_get_smsc(dev_name, &number);
	if(stat != CG_STATUS_OK) {
		fprintf(stderr, "Can't get sms service center (%d)\n",stat);
		goto exit;
	}
	printf ( "Service center number set %s\n",number);
	exitcode = 0;
exit:
	cg_deinit();
	free (number);
	free (dev_name );
	return exitcode;
}
