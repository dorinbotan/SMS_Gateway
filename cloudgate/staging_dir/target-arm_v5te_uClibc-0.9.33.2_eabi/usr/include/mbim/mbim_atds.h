#ifndef MBIM_ATDS_H
#define MBIM_ATDS_H

#include <stdlib.h>

#include <mbim/mbim.h>

/**
 * \file mbim_atds.h
 * \brief Public header file which contains all command id functionality for the
 * atds services.
 */

/**
 * \defgroup Atds functions (PUBLIC)
 * @{
 */

/**
 * \struct mbim_location_info_t
 * \brief The current location information for the modem.
 */
typedef struct {
	/** Location area code. 0 if not supported. */
	uint32_t lac;
	/** The TAC value. 0 if not supported. */
	uint32_t tac;
	/** Cell identifier */
	uint32_t cellid;
} mbim_atds_location_info_t;

uint32_t mbim_get_atds_location_info(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * \struct mbim_atds_signal_info_t
 * \brief Signal information from the modem.
 */
typedef struct {
	/** RSSI */
	uint32_t rssi;
	/** Bit error rate */
	uint32_t ber;
	/** Received signal code power */
	uint32_t rscp;
	/** EC/NO */
	uint32_t ecno;
	/** Reference signal received quality */
	uint32_t rsrq;
	/** Reference signal received power */
	uint32_t rsrp;
	/** The signal noise ratio for LTE */
	uint32_t rssnr;
} mbim_atds_signal_info_t;

uint32_t mbim_get_atds_signal_info(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/** @} */

#endif
