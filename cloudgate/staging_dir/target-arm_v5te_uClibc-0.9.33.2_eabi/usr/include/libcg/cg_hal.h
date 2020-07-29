#ifndef __CG_HAL_H__
#define __CG_HAL_H__

#include <libcg/cgate.h>

CG_BEGIN_DECLS

/**
* \file cg_hal.h
* \brief Header file containing all definitions/functions related specific to the Hardware Abstraction Layer.
*/

/**
* \defgroup hal HAL
* @{
*/

/**
* \enum cg_hal_serial_mode_t
* \brief modes for RS485 or RS232
*/
typedef enum {
	/** RS232 mode */
	CG_HAL_SERIAL_RS232 = 0x1,
	/** RS485 mode */
	CG_HAL_SERIAL_RS485 = 0x2,
} cg_hal_serial_mode_t;

/**
* \struct cg_hal_serial_rs485_cfg_t
* \brief Serial RS485-specific configuration settings
*/
typedef struct {
	/** Enable the termination resistors on the RS485 wires */
	uint32_t enable_termination;
} cg_hal_serial_rs485_cfg_t;

/**
* \struct cg_hal_serial_cfg_t
* \brief Generic configuration settings for a serial interface
*/
typedef struct {
	/** Serial mode selection between RS485 or RS232 */
	cg_hal_serial_mode_t serial_mode;
	/** Configuration specific to the selected serial mode (if required) */
	union {
		/** RS232 has no additional parameters */
		/** RS485 configuration parameters */
		cg_hal_serial_rs485_cfg_t rs485;
	} param;
} cg_hal_serial_cfg_t;

/** \name SERIAL - specific functions
* @{ */

/** Set the serial interface configuration for RS485 or RS232
* \param[in] dev_name SERIAL device name
* \param[in] config parameters
* \param[out] reboot_needed indication that a reboot is needed before the new configuration is used.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or device name or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_hal_serial_set_config(const char *dev_name, const cg_hal_serial_cfg_t *config, int *reboot_needed);

/** @} */

/** @} */

CG_END_DECLS

#endif /* __CG_HAL_H__ */
