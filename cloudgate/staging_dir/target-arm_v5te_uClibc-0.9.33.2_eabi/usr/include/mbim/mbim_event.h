#ifndef MBIM_EVENT_H
#define MBIM_EVENT_H

#include <string.h>

#include <mbim/mbim.h>

/**
* \file mbim_event.h
* \brief header file which describes the public event handling function when
* receiving data from the MBIM device.
*/

/**
* \defgroup events General event handling (PUBLIC)
* @{
*/

/**
 * \enum mbim_event_type_t based on mbim_control_message_t
 * \brief Parameter for the mbim_handle_event function.
 */
typedef enum {
	/** Internally used when opening the mbim device */
	MBIM_EVENT_OPEN_DONE = 1,
	/** Internally used when closing the mbim device */
	MBIM_EVENT_CLOSE_DONE,
	/** Reply after a command has been sent to the mbim device */
	MBIM_EVENT_COMMAND_DONE,
	/** An error occured in the mbim device */
	MBIM_EVENT_FUNCTION_ERROR_MSG,
	/** Unsolicited events */
	MBIM_EVENT_INDICATE_STATUS_MSG = 7,
	/** Handle all events. Recommended for clients who use the libmbim-c library. */
	MBIM_EVENT_ANY
} mbim_event_type_t;

/**
 * General function to handle incoming events.
 * The minimum requirement for the data is that the mbim message header is
 * included. Search for the transaction id in the list and call the specific
 * event handler function.
 * \param[in] ctx The mbim device context
 * \param[in] control_message The message type we want to handle. E.g. MBIM_CONTROL_MSG_ANY handles
 * all events. MBIM_OPEN_DONE handles the open done messages and ignores the other ones.
 * \return read errror: MBIM_ERROR, parse failures, MBIM_INVALID_PARAMETER, otherwise MBIM_OK
 */
uint32_t mbim_handle_event(mbim_context_t *ctx, mbim_event_type_t event_type);

/** @} */

#endif
