#ifndef MBIM_IMC_H
#define MBIM_IMC_H

#include <stdlib.h>

#include <mbim/mbim.h>

/**
 * \file mbim_imc.h
 * \brief Public header file which contains all command id functionality for the
 * Intel Mobile Communication services.
 */

/**
 * \defgroup IMC functions (PUBLIC)
 * @{
 */

/**
 * Reboot the module into firmware update mode.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_intc_modem_reboot(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/** @} */

#endif
