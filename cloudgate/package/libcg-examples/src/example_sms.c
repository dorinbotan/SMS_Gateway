#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>
#include <libcg/cg_sms.h>

#include <libsms/sms_cdma.h>
#include <libsms/sms_pdu.h>

#define NUMBER "+4529176086"

static cg_wwan_mode_t mode;
uint16_t id_cdma = 0;
uint8_t id_pdu = 0;
char * dev_name = NULL;

// SMS function call returns CG_STATUS_ERROR if primary devices is Hpex, because it doesn't support SMS yet.
static void cb(const char *dev_name, uint16_t sms_size, unsigned char *sms, void* context)
{
	int i;
	printf("\nNEW SMS: Raw message:\n");
	for(i = 0; i < sms_size; i++)
		printf("%02x", sms[i]);
	printf("\n");

	if(mode == CG_WWAN_MODE_CDMA) {
		printf("------ 1");
		sms_cdma_data_t* data = cdma_pdu_parse(sms,sms_size);
		printf("Sender: %s\n",data->sender);
		printf("Text: %s\n",data->text);
		free(data);
	} else {
		printf("------ 2");
		char* sms_hex = calloc(1,(2*sms_size) + 1);
		int i;
		for( i = 0; i < sms_size; i++) {
			sprintf(&sms_hex[2*i],"%02x", sms[i]);
		}
		sms_data_t data = sms_pdu_parse(sms_hex);
		printf("Sender: %s\n",data.sender);
		printf("Text: %s\n",data.text);
		free(sms_hex);
	}
}

int main(int argc, char *argv[])
{
	cg_status_t stat;
	int32_t level;

	cg_init("SMS example");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if (cg_wwan_get_active_primary_dev ( &dev_name ) != CG_STATUS_OK) {
		cg_deinit();
		printf ("Failed to get active primary device\n");
		return 0;
	}
	printf ("Active primary WWAN device: %s\n", dev_name);

	stat = cg_wwan_get_mode(dev_name, &mode);
	if(stat != CG_STATUS_OK)
		goto exit;

	stat = cg_sms_register_new_sms(dev_name, cb, NULL);
	if(stat != CG_STATUS_OK)
		goto exit;

	char number[] = NUMBER;
	printf("%s\n", number);

	while(1)
	{
		char text[160];
		fputs("enter text: ", stdout);
		fflush(stdout);
		if ( fgets(text, sizeof(text), stdin) != NULL )
		{
			if(text[0] == '\n')
				continue;

			char *newline = strchr(text, '\n'); /* search for newline character */
			if ( newline != NULL )
			{
				*newline = '\0'; /* overwrite trailing newline */
			}
		}
		if(mode == CG_WWAN_MODE_CDMA) {
			uint8_t* sms;
			uint8_t sms_size = sms_cdma_create(text, number, id_cdma++, 0, 0, &sms);
			//send sms
			stat = cg_sms_send(dev_name,sms_size,sms);
			if(stat != CG_STATUS_OK) {
				printf("1 Error sending SMS...\n");
			} else {
				printf("1 Message sent\n");
			}
			free(sms);

		} else {
			pdu_list_t* pdu_list = sms_pdu_create(text, number, NULL, id_pdu, 0, 0);
			int i;

			if(pdu_list==NULL)
			{
				printf("Error creating SMS...\n");
				continue;
			}

			for(i=0; i < pdu_list->length; i++) {
				pdu_data_t *pdu = &pdu_list->pdu[i];
				//send sms
				stat = cg_sms_send(dev_name, pdu->size, pdu->data);

				if(stat != CG_STATUS_OK) {
					printf("2 Error (%d != %d) sending SMS...\n", stat, CG_STATUS_OK);
				} else {
					printf("2 Message sent\n");
				}
				free(pdu->data);
			}
			free(pdu_list);
		}
	}
exit:
	cg_deinit();
	free (dev_name );
	return 0;
}
