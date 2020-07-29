#ifndef MBIM_STK_H
#define MBIM_STK_H

#include <stdlib.h>

#include <mbim/mbim.h>

/**
 * \file mbim_stk.h
 * \brief Public header file which contains all command id functionality for the SIM Toolkit
 */

/**
 * \defgroup STK functions (PUBLIC)
 * @{
 */

/**
 * \enum mbim_stk_pac_profile
 * \brief Terminal profile which identiefies the current support for a specific proactive command.
 */
typedef enum {
	/** The command is not handled in the function and the function will not be able to forward the command to the host. */
	MBIM_STK_NOT_HANDLED_BY_FUNCTION_CANNOT_BE_HANDLED_BY_HOST,
	/** The command is not handled by function but may be handledby the host if host request so with the set command. */
	MBIM_STK_NOT_HANDLED_BY_FUNCTION_MAY_BE_HANDLED_BY_HOST,
	/** The command is handled by function without informing the host. The function will not allow the host to handle the command or receive notifications of the command even if the host requests so.*/
	MBIM_STK_HANDLED_BY_FUNCTION_ONLY_TRANSPARENT_TO_HOST,
	MBIM_STK_HANDLED_BY_FUNCTION_NOTIFICATION_TO_HOST_POSSIBLE,
	MBIM_STK_HANDLED_BY_FUNCTION_NOTIFICATIONS_TO_HOST_ENABLED,
	MBIM_STK_HANDLED_BY_FUNCTION_CAN_BE_OVERRIDDEN_BY_HOST,
	MBIM_STK_HANDLED_BY_HOST_FUNCTION_NOT_ABLE_TO_HANDLE,
	MBIM_STK_HANDLED_BY_HOST_FUNCTION_ABLE_TO_HANDLE
} mbim_stk_pac_profile_t;

/**
 * \enum mbim_stk_pac_type
 * \brief Type of the proactive command.
 */
typedef enum {
	/** The proactive command is a proactive command that the host is requested to handle. */
	MBIM_STK_PROACTIVE_COMMAND,
	/** The proactive command is handled by the function, but the host is notified that the SIM has sent the proactive command and may decide to e.g. indicate this to the end user. */
	MBIM_STK_NOTIFICATION
} mbim_stk_pac_type_t;

/**
 * \struct mbim_stk_pac_info
 * \brief Struct returned after a set or query for MBIM_CID_STK_PAC.
 */
typedef struct {
	/** 256 bytes where each byte identifies the current support for a specific proactive function. \
	 * Byte A in the string tells the current support for proactive command A. The numbering of the \
	 * commands will be according to the values associated with each command in ETSI 102 223 V9.0.0. \
	 * e.g. byte 1 means REFRESH, byte 21 means DISPLAY TEXT. Value refers to mbim_stk_pac_profile_t */
	uint8_t pac_support[256];
} mbim_stk_pac_info_t;

/*
 * \struct mbim_stk_pac_t
 * \brief Struct returned after a notification is received for MBIM_CID_STK_PAC.
 */
typedef struct {
	/** The type of the proactive command */
	mbim_stk_pac_type_t pac_type;
	/** Proactive command in BER-TLV format, starting with the proactive UICC Command tag (0xD0). */
	uint8_t data_buffer[];
} mbim_stk_pac_t;

/**
 * Request the proactive commands from the SIM toolkit.
 * \param[in] ctx pointer to the context of the mbim device
 * \param[in] cb user callback to be called when the visible providers are received
 * \param[in] arg extra arguments.
 */
uint32_t mbim_get_stk_pac(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/** @} */

#endif
