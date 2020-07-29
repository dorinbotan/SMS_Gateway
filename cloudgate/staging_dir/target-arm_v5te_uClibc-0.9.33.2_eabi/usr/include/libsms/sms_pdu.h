#ifndef __SMS_PDU_H__
#define __SMS_PDU_H__

#include <stdint.h>
#include <time.h>

/**
 * \file sms_pdu.h
 * \brief Header file with definitions for creating and parsing 3GPP (GSM/UMTS) 23.040 PDUs
 */

/**
 * \defgroup sms_pdu SMS_PDU
 * @{
 */

/**
 * \enum dcs_coding_t
 * \brief dcs coding type seen by the parser
 */
typedef enum {
	/** class 0, default gsm 7bit */
	DCS_CLASS0_DEFAULT_ALPHABET,
	/** class 1, default gsm 7bit */
	DCS_CLASS1_DEFAULT_ALPHABET,
	/** class 2, default gsm 7bit */
	DCS_CLASS2_DEFAULT_ALPHABET,
	/** class 0, unidentified alphabet, could be ucs2 */
	DCS_CLASS0_UNSUPPORTED_ALPHABET,
	/** class 1, unidentified alphabet, could be ucs2 */
	DCS_CLASS1_UNSUPPORTED_ALPHABET,
	/** class 2, unidentified alphabet, could be ucs2 */
	DCS_CLASS2_UNSUPPORTED_ALPHABET,
	/** an unsupported class was detected (e.g. compressed, message waiting indication, class > 2,...) */
	DCS_UNSUPPORTED_CLASS,
} dcs_coding_t;

/**
 * \struct sms_pdu_data_t
 * \brief structure returned by parsing, containing pdu information
 * \todo move decoded text to the back
 */
typedef struct {
	/** Service Center Address if included */
	unsigned char  serviceCenter[64];
	/** Sender's Address */
	unsigned char  sender[64];
	/** Date of sms */
	unsigned char  date[10];
	/** Time of sms */
	unsigned char  time[10];
	/** Decoded text */
	unsigned char  text[160*4+1]; // 160 chars and worst case utf-8 is 8 bytes per char
	/** time_t object of date/time of sms */
	time_t         ctime;
	/** Sms concatenation: part count (parsed from 23.040 IEI 0x00: 8-bit reference number) */
	unsigned int   part_count;    /* Number of parts in this SMS */
	/** Sms concatenation: part index (parsed from 23.040 IEI 0x00: 8-bit reference number) */
	unsigned int   part_index;    /* Current part number */
	/** Sms long reference number: part count (parsed from 23.040 IEI 0x00: 8-bit reference number) */
	unsigned int   part_refno;
	/** Message Type indication (23.040 TP-MTI) */
	unsigned char  mti;
	/** Protocol Identifier (23.040 TP-PID) */
	unsigned char  tp_pid;
	/** Parsed coding type */
	dcs_coding_t   coding;
} sms_data_t;

/**
 * \struct pdu_data_t
 * \brief structure containing the actual pdu data
 */
typedef struct {
	/** Size of data in octects */
	int size;
	/** 23.040 encoded pdu, user must free the data pointer after use*/
	unsigned char* data;
} pdu_data_t;

/**
 * \struct pdu_list_t
 * \brief list of pdus returened by pdu encoder
 */
typedef struct {
	/** number of pdus created */
	int length;
	/** array of pdus */
	pdu_data_t pdu[];
} pdu_list_t;

/** Create a (set of) pdu(s)
 * \param[in] msg The message to be encoded
 * \param[in] number The destination address
 * \param[in] smsc Optional Service center address
 * \param[in] id unique message id (used in case of concatenation)
 * \param[in] flash if 0, a CLASS 1 message will be created, if 1, a CLASS 0 message will be created
 * \param[in] force_ucs2 if 1, the default encoding will be UCS2
 * \return NULL in case of error, and a pdu_list_t object otherwise, user must free the object after use (and also the data pointers of the pdus)
 */
pdu_list_t* sms_pdu_create(char *msg, char *number, char* smsc, uint8_t id, int flash, int force_ucs2);

/** Parse a pdu
 * \param[in] pdu_hex the pdu in "hex string format"
 * \return sms_data_t all information of the supplied PDU
 * \todo change the return value into a pointer that needs to be freed by the user instead of putting it on the stack
 * \todo change pdu_hex into a unsigned char buffer and an extra size param (int)
 */
sms_data_t sms_pdu_parse(char *pdu_hex);

/** encode a smsc_number into 23.040 scaBytes
 * \param[in] smsc_number a null terminated string containing the smsc
 * \param[in,out] scaBytes a buffer where the encoded service center will be put
 * \return number of bytes put in scaBytes
 */
int addr2scabytes(char* smsc_number, unsigned char *scaBytes);

/** converts a hex string to binary array
 * \param[in] hex a null terminated string containing hex values
 * \param[out] bin an array of bytes to store the converted binary values
 */
void parse_hex(char *hex, unsigned char *bin);
/** @} */
#endif
