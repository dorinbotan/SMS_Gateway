#include "sms.h"

#include <stdlib.h>
#include <stdio.h>
#include <libcg/cg_general.h>
#include <libcg/cg_wwan.h>
#include <libcg/cg_sms.h>
#include <libsms/sms_cdma.h>
#include <libsms/sms_pdu.h>

cg_wwan_mode_t mode;
char *dev_name = NULL;

sms_receive_callback_t sms_receive_callback = NULL;

void _sms_receive_callback(const char *dev_name, uint16_t sms_size, unsigned char *sms, void *context)
{
    if (!sms_receive_callback)
        return;
        
    if (mode == CG_WWAN_MODE_CDMA)
    {
        sms_cdma_data_t *data = cdma_pdu_parse(sms, sms_size);

        sms_receive_callback((const char *)data->sender, (const char *)data->text);

        free(data);
    }
    else
    {
        char *sms_hex = calloc(1, (2 * sms_size) + 1);

        for (int i = 0; i < sms_size; i++)
            sprintf(&sms_hex[2 * i], "%02x", sms[i]);

        sms_data_t data = sms_pdu_parse(sms_hex);

        sms_receive_callback((const char *)data.sender, (const char *)data.text);

        free(sms_hex);
    }
}

int sms_init()
{
    if (cg_wwan_get_active_primary_dev(&dev_name) != CG_STATUS_OK)
        return 0;

    if (cg_wwan_get_mode(dev_name, &mode) != CG_STATUS_OK)
        return 0;

    return 1;
}

int sms_send(char *number, char *text)
{
    static uint16_t id_cdma = 0;
    static uint8_t id_pdu = 0;

    if (mode == CG_WWAN_MODE_CDMA)
    {
        uint8_t *sms;
        uint8_t sms_size = sms_cdma_create(text, number, id_cdma++, 0, 0, &sms);
        cg_status_t stat = cg_sms_send(dev_name, sms_size, sms);

        free(sms);

        if (stat != CG_STATUS_OK)
            return 0;
    }
    else
    {
        pdu_list_t *pdu_list = sms_pdu_create(text, number, NULL, id_pdu, 0, 0);

        if (pdu_list == NULL)
            return 0;

        for (int i = 0; i < pdu_list->length; i++)
        {
            pdu_data_t *pdu = &pdu_list->pdu[i];
            cg_status_t stat = cg_sms_send(dev_name, pdu->size, pdu->data);

            free(pdu->data);

            if (stat != CG_STATUS_OK)
                return 0;
        }

        free(pdu_list);
    }

    return 1;
}

int sms_onreceive(sms_receive_callback_t callback)
{
    sms_receive_callback = callback;
    return (cg_sms_register_new_sms(dev_name, _sms_receive_callback, NULL) == CG_STATUS_OK);
}

void sms_deinit()
{
    free(dev_name);
}
