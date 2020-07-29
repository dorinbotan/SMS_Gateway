#ifndef __CG_SMS_H__
#define __CG_SMS_H__

#include <libcg/cgate.h>

CG_BEGIN_DECLS

/**
* \file cg_sms.h
* \brief Header file containing all definitions/functions related specific to SMS. 
*/

/**
* \defgroup sms SMS
* @{
*/

/** Callback for receiving new sms's
* \param[in] dev_name WWAN device name
* \param[in] sms_size size in bytes of the sms
* \param[in] sms The SMS, in layer3 format for 3GPP2(CDMA) and in PDU format for 3GPP (GSM/UMTS)
* \param[in] context User-defined context.
*/
typedef void (*cg_sms_cb_t) (const char *dev_name, uint16_t sms_size, unsigned char *sms, void* context);

/** Set the SMS Message Center number
 *  this is invalid for CDMA
* \param[in] dev_name WWAN device name
* \param[in] number the SMSC number, eg. +3212345678. An empty smsc ("") will clear the configured smsc and go back to the SIM's default.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error. Invalid paramater include a too long smsc number (>20 digits) and invalid dev_names.
*/
cg_status_t cg_sms_set_smsc(const char *dev_name, const char *number);

/** Get the SMS Message Center number
* \param[in] dev_name WWAN device name
* \param[out] number The SMSC number, eg. +3212345678. If no smsc is configured and the device isn't ready reading it from the SIM, "unknown" will be returned as smsc.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_sms_get_smsc(const char *dev_name, char **number);

/** Register a callback for new SMS notifications
* \param[in] dev_name WWAN device name
* \param[in] cb New SMS callback.
* \param[in] context User-defined context passed on to cb when called. 
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_sms_register_new_sms(const char *dev_name, cg_sms_cb_t cb, void *context);

/** Deregister a callback for new SMS notifications
* \param[in] dev_name WWAN device name
* \param[in] cb Callback.
* \param[in] context User-defined context passed on to cb when called. 
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_sms_deregister_new_sms(const char *dev_name, cg_sms_cb_t cb, void *context);

/** Send an SMS. The function will cut the SMS in parts if it is too large to fit in a single part.
* \param[in] dev_name WWAN device name
* \param[in] sms_size size in bytes of the sms
* \param[in] sms The SMS, in layer3 format for 3GPP2(CDMA) and in PDU format for 3GPP (GSM/UMTS)
 * For 3GPP the raw message in PDU format must include the SMSC address length identiﬁer as the ﬁrst byte of the message. If this byte is
 * set to zero, the SMSC set with cg_sms_set_smsc is used, or the default (SIM) otherwise. Otherwise, the ﬁrst byte indicates the length, in bytes, of the
 * SMSC address that is included after the ﬁrst byte, but before the start of the actual PDU message.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_sms_send(const char *dev_name, uint16_t sms_size, const unsigned char* sms);


/** @} */

CG_END_DECLS

#endif /* __CG_SMS_H__ */
