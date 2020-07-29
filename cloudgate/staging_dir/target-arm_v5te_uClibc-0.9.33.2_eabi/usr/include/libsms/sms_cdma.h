#ifndef __SMS_CDMA_H
#define __SMS_CDMA_H

#include <stdint.h>
#include <time.h>

/**
 * \file sms_cdma.h
 * \brief Header file with definitions for creating and parsing 3GPP2 (CDMA) Layer 3 SMS (C.S0015)
 */

/**
 * \defgroup sms_cdma SMS_CDMA
 * @{
 */

/**
 * \enum sms_cdma_coding_t
 * \brief encoding seen by the parser
 * \todo support other encoding then 7bit ascii
 */
typedef enum  {
	/** (NOT DECODED) */
	SMS_CDMA_CODING_OCTET =                   0,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_EXTENDED_PROTOCOL_MSG =   1,
	/** 7bit ASCII */
	SMS_CDMA_CODING_7BIT_ASCII =              2,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_IA5 =                     3,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_UNICODE =                 4,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_SHIFT_JIS =               5,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_KOREAN =                  6,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_LATIN_HEBREW =            7,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_LATIN =                   8,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_GSM_7BIT =                9,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_GSM_DATA_CODING =        10,
	/** (NOT DECODED) */
	SMS_CDMA_CODING_UNKNOWN =               0xFF
}sms_cdma_coding_t;

/**
 * \struct sms_pdu_data_t
 * \brief structure returned by parsing, containing pdu information
 */
typedef struct {
	/** Sender's Address */
	char                     sender[64];
	/** Date of sms */
	unsigned char            date[10];
	/** Time of sms */
	unsigned char            time[10];
	/** time_t object of date/time of sms */
	time_t                   ctime;
	/** (NOT IMPLEMENTED) Sms concatenation: part count (parsed from 23.040 IEI 0x00: 8-bit reference number) */
	unsigned int             part_count;    /* Number of parts in this SMS */
	/** (NOT IMPLEMENTED) Sms concatenation: part index (parsed from 23.040 IEI 0x00: 8-bit reference number) */
	unsigned int             part_index;    /* Current part number */
	/** (NOT IMPLEMENTED) Sms long reference number: part count (parsed from 23.040 IEI 0x00: 8-bit reference number) */
	unsigned int             part_refno;    /* The long SMS reference number. should be unique (how could you with a char?). */
	/** Parsed coding type */
	sms_cdma_coding_t  coding;
	/** Decoded text */
	char                     text[256];
} sms_cdma_data_t;

/** Create a (set of) pdu(s)
 * \param[in] msg The message to be encoded
 * \param[in] number The destination address
 * \param[in] id unique message id, on first use, the user should select an id using a procedure that minimizes the likehood of reusing the same id (see C.S0015 4.3.1.5)
 * \param[in] flash (NOT IMPLEMENTED) if 0, a CLASS 1 messaage will be created, if 1, a CLASS 0 message will be created
 * \param[in] force_ucs2  (NOT IMPLEMENTED) if 1, the default encoding will be UCS2
 * \return NULL in case of error, and a pdu_list_t object otherwise, user must free the object after use (and also the data pointers of the pdus)
 */
uint8_t sms_cdma_create(char *msg, char *number, uint16_t id, int flash, int force_ucs2, uint8_t** buf);

/** Parse a pdu
 * \param[in] pdu the raw pdu
 * \param[in] size the size of the raw pdu in octets
 * \return sms_cdma_data_t* all information of the supplied sms, needs to be freed by the user after use
 */
sms_cdma_data_t* cdma_pdu_parse(uint8_t *pdu, uint32_t size);
/** @} */

#endif
