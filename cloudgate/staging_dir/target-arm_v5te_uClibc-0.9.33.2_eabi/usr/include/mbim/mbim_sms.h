#ifndef MBIM_SMS_H
#define MBIM_SMS_H

#include <libsms/sms_pdu.h>

/**
 * \file mbim_sms.h
 * \brief Public header file which contains all command id functionality for the
 * sms services
 */

/**
 * \defgroup sms_services SMS functions (PUBLIC)
 * @{
 */

/**
 * \enum mbim_sms_storage_state_t
 * \brief The state of the device's sms storage
 */
typedef enum {
	MBIM_SMS_STORAGE_NOT_INITIALIZED,
	MBIM_SMS_STORAGE_INITIALIZED
} mbim_sms_storage_state_t;

/**
 * \enum mbim_sms_format_t
 * \brief A multi-mode single-carrier function must use only GSM PDU format (MBIMSmsFormatPdu) even if the
 underlying cellular technology switches from GSM to CDMA, and must provide the appropriate
 transcoding in the send and receive paths. This applies to both PDU and CDMA text formats.
 */
typedef enum {
	MBIM_SMS_FORMAT_PDU,
	MBIM_SMS_FORMAT_CDMA
} mbim_sms_format_t;

/**
 * \struct mbim_set_sms_configuration_t
 * \brief The struct used in a set command for cid MBIM_CID_SMS_CONFIGURATION.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** The sms format */
	mbim_sms_format_t format;
	/** Offset from the beginning of the struct to a string which represents the Service Center number. */
	uint32_t sc_address_offset;
	/** The size of the Service Center number string. Max size is 40 bytes. */
	uint32_t sc_address_size;
	/** The databuffer which contains the Service Center number in UTF-16 format */
	uint8_t data_buffer[];
} mbim_set_sms_configuration_t;

/**
 * \struct mbim_sms_configuration_info_t
 * \brief Response struct after a MBIM_CID_SMS_CONFIGURATION command/query is executed or when a notification comes in.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** The current sate of the SMS storage */
	mbim_sms_storage_state_t sms_storage_state;
	/** The sms format */
	mbim_sms_format_t format;
	/** The maximum number of messages that can be stored on the device */
	uint32_t max_messages;
	/** The maximum SMS character length that is supported by the device, if the device is CDMA-based */
	uint32_t cdma_short_message_size;
	/** The Service Center number in UTF-8. Maximum 20 bytes. */
	char *sc_number;
} mbim_sms_configuration_info_t;

/**
 * Query the SMS configuration.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback.
 * \param[in] arg Extra argument for the user callback.
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_sms_configuration(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * Set the SMS configuration.
 * \param[in] ctx The mbim device context.
 * \param[in] format The sms format. PDU or CDMA.
 * \param[in] sc_number The service center number as string. Two possibles values:
 * 1) "+<international country code><SMS service center number>"
 * 2) "<SMS service center number>"
 * \note The maximum size is 20 characters.
 * \param[in] cb User data callback.
 * \param[in] arg Extra argument for the user callback.
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_set_sms_configuration(mbim_context_t *ctx, mbim_sms_format_t format, char *sc_number, mbim_user_data_cb cb, void *arg);

/**
 * \enum mbim_sms_flag_t
 * \brief Flags can be used as a filter when querying the text messages for cid MBIM_CID_SMS_READ.
 */
typedef enum {
	MBIM_SMS_FLAG_ALL,
	MBIM_SMS_FLAG_INDEX,
	MBIM_SMS_FLAG_NEW,
	MBIM_SMS_FLAG_OLD,
	MBIM_SMS_FLAG_SENT,
	MBIM_SMS_FLAG_DRAFT,
	MBIM_SMS_FLAG_MAX
} mbim_sms_flag_t;

/**
 * \enum mbim_sms_message_status_t
 * \brief The status of a sms message.
 */
typedef enum {
	MBIM_SMS_STATUS_NEW,
	MBIM_SMS_STATUS_OLD,
	MBIM_SMS_STATUS_DRAFT,
	MBIM_SMS_STATUS_SENT
} mbim_sms_message_status_t;

/**
 * \struct mbim_sms_pdu_record_t
 * \brief struct for PDU messages.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** An index into the virtual message store that is maintained by the MBIM device.
	 * The index is 1-based and the maximum index is returned in max_messages for the cid
	 * MBIM_CID_SMS_CONFIGURATION */
	uint32_t message_index;
	/** The status of the sms message */
	mbim_sms_message_status_t message_status;
	/** Offset in bytes from the beginning of this struct to the PDU representation of the sms message. */
	uint32_t pdu_data_offset;
	/** The size, in bytes, of the PDU data. GSM: max 183. CDMA: max 255. */
	uint32_t pdu_data_size;
	/** The databuffer which contains the PDU data */
	uint8_t data_buffer[];
} mbim_sms_pdu_record_t;

/**
 * \struct mbim_sms_pdu_t
 * \brief Describes one PDU record in a reply after a query or notification of MBIM_CID_SMS_READ. This
 * structure is based on mbim_sms_pdu_record_t.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** An index into the virtual message store that is maintained by the MBIM device.
	 * The index is 1-based and the maximum index is returned in max_messages for the cid
	 * MBIM_CID_SMS_CONFIGURATION */
	uint32_t message_index;
	/** The status of the sms message */
	mbim_sms_message_status_t message_status;
	/** The size of the PDU data */
	uint32_t size;
	/** Pointer to the PDU data */
	uint8_t *data;
} mbim_sms_pdu_t;

/**
 * \struct mbim_sms_cdma_t
 * \brief Describes one CDMA record in a reply after a query or notification of MBIM_CID_SMS_READ.
 * CDMA is not implemented yet.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	uint32_t not_implemented;
} mbim_sms_cdma_t;

typedef struct {
	/** The SMS format. PDU or CDMA */
	mbim_sms_format_t format;
	/** The number of elements in the format array */
	uint32_t count;
	/** Pointer to an array of mbim_sms_pdu_t structures. Only valid if format is MBIM_SMS_FORMAT_PDU */
	mbim_sms_pdu_t *pdu_records;
	/** Pointer to an array of mbim_sms_cdma_t structures. Only valid if format is MBIM_SMS_FORMAT_CDMA */
	mbim_sms_cdma_t *cdma_records;
} mbim_sms_read_info_t;

/**
 * \struct mbim_sms_read_req
 * \brief Struct used when querying the cid MBIM_CID_SMS_READ.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** The sms format */
	mbim_sms_format_t sms_format;
	/** The sms flag which can be used as a filter */
	mbim_sms_flag_t flag;
	/** A value between 1 and max_messages that is an index into the device's message store. This
	 * value is valid ony if flag is set to MBIM_SMS_FLAG_INDEX. The host sets this member if flag
	 * is set to MBIM_SMS_FLAG_INDEX. For all other flags, the host sets this index to 0. */
	uint32_t message_index;
} mbim_sms_read_req_t;

/**
 * \struct mbim_sms_read_info_pdu_t
 * \brief Response from the MBIM device for a notification or after a query for cid MBIM_CID_SMS_READ if
 * the sms format is PDU.
 **/
typedef struct __attribute__ ((packed, aligned(4))) {
	/** The sms format */
	mbim_sms_format_t format;
	/** The number of elements in the array */
	uint32_t element_count;
	/** The array which contains struct dependent on the sms format. PDU or CDMA */
	mbim_sms_pdu_record_t pdu_record[];
} mbim_sms_read_info_pdu_t;

//mbim_sms_read_info_cdma_t not implemented!

/**
 * Read the SMS messages from the MBIM device and/or SIM.
 * \param[in] ctx The mbim device context.
 * \param[in] format The sms format. PDU or CDMA.
 * \param[in] flag Determines which SMS messages to read. Acts as a filter.
 * \param[in] message_index. The index into the device's message store. Only used when flag is set to MBIM_SMS_FLAG_INDEX.
 * \param[in] cb User data callback.
 * \param[in] arg Extra argument for the user callback.
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_read_sms_messages(mbim_context_t *ctx, mbim_sms_format_t format,
		mbim_sms_flag_t flag, uint32_t message_index, mbim_user_data_cb cb, void *arg);

/**
 * \struct mbim_sms_send_pdu_t
 * \brief Struct used as databuffer when sending a sms via cid MBIM_CID_SMS_SEND and the sms format is PDU.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** Offset in bytes from the beginning of this struct to the PDU databuffer */
	uint32_t pdu_data_offset;
	/** The size, in bytes, of the PDU data. GSM: max 183 bytes. CDMA: max 255 bytes */
	uint32_t pdu_data_size;
	/** The PDU databuffer */
	uint8_t pdu_data[];
} mbim_sms_send_pdu_t;

/**
 * \struct mbim_set_sms_send_t
 * \brief Struct used for sending a sms via cid MBIM_CID_SMS_SEND.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** The sms format */
	mbim_sms_format_t sms_format;
	/** The databuffer. If format is PDU, this is a mbim_sms_send_pdu_t struct. If format is CDMA, this is not implemented */
	uint8_t data_buffer[];
} mbim_set_sms_send_t;

/**
 * \struct mbim_sms_send_info_t
 * \brief Response struct after sending a sms via cid MBIM_CID_SMS_SEND.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** A reference number that is returned on successful delivery of the SMS */
	uint32_t message_reference;
} mbim_sms_send_info_t;

/**
 * Send a SMS in PDU format. The user supplies the PDU data.
 * \param[in] ctx The mbim device context.
 * \param[in] pdu_data Pointer to the PDU data.
 * \param[in] pdu_size The size of the PDU data. Maximum size is 183 bytes.
 * \param[in] cb User data callback.
 * \param[in] arg Extra argument for the user callback.
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_send_sms_pdu(mbim_context_t *ctx, uint8_t *pdu_data, uint32_t pdu_size, mbim_user_data_cb cb, void *arg);

/**
 * Create a PDU sms message.
 * \param[in] ctx The mbim device context.
 * \param[in] message The message to send.
 * \param[in] number The phone number were the SMS must be send to.
 * \param[in] smsc The service center number (optional).
 * \param[in] ref_number A reference number for the SMS.
 * \param[in] flash If zero, a class 1 SMS will be created, otherwise a class 0 (flash) SMS.
 * \param[in] force_ucs2 If one, the message is converted to GSM format.
 * \param[out] pdu_list The created PDU message. The caller must free the pdu_list and all pdu_data_t data pointers after usage.
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_sms_create_pdu(mbim_context_t *ctx, char *message, char *number, char *smsc,
		uint8_t ref_number, uint32_t flash, uint32_t force_ucs2, pdu_list_t **pdu_list);

/**
 * \struct mbim_set_sms_delete_t
 * \brief Struct used for deleting sms messages for the cid MBIM_CID_SMS_DELETE.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** The sms flag which can be used as a filter */
	mbim_sms_flag_t flag;
	/** A value between 1 and max_messages that is an index into the device's message store. This
	 * value is valid ony if flag is set to MBIM_SMS_FLAG_INDEX. The host sets this member if flag
	 * is set to MBIM_SMS_FLAG_INDEX. For all other flags, the host sets this index to 0. */
	uint32_t message_index;
} mbim_set_sms_delete_t;

/**
 * Set SMS Delete.
 * \param[in] ctx The mbim device context.
 * \param[in] sms_flag which message group to be deleted
 * \param[in] sms_index when the sms_flag is set to MBIM_SMS_FLAG_INDEX, the message on the indicated memory index needs to be deleted 
 * \param[in] cb User data callback.
 * \param[in] arg Extra argument for the user callback.
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_set_sms_delete(mbim_context_t *ctx, mbim_sms_flag_t sms_flag, uint32_t sms_index, mbim_user_data_cb cb, void *arg);

/**
 * \enum mbim_sms_status_flags_t
 * \brief The sms status flags used for cid MBIM_CID_SMS_MESSAGE_STORE_STATUS.
 */
typedef enum {
	/** No status to report */
	MBIM_SMS_FLAG_NONE,
	/** The message store is full */
	MBIM_SMS_FLAG_MESSAGE_STORE_FULL,
	/** A new, non-class 0 (flash/alert) message has arrived */
	MBIM_SMS_FLAG_NEW_MESSAGE
} mbim_sms_status_flags_t;

/**
 * \struct mbim_sms_status_info_t
 * \brief Response after querying the cid MBIM_CID_SMS_MESSAGE_STORE_STATUS or when a notification comes in.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** The sms status flag */
	mbim_sms_status_flags_t flag;
	/** This is the index of the newly arrived message or the recently arrived message in case of a query response */
	uint32_t message_index;
} mbim_sms_status_info_t;

/**
 * Query the SMS message store status. This can also indicate if there are new SMS messages available.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback.
 * \param[in] arg Extra argument for the user callback.
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_sms_message_store_status(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/** @} */

#endif
