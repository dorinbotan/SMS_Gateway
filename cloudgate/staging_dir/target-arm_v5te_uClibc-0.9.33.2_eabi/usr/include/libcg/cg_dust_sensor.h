#ifndef __CG_DUST_SENSOR_H__
#define __CG_DUST_SENSOR_H__
#include <libcg/cgate.h>

CG_BEGIN_DECLS

/**
* \file cg_dust_sensor.h
* \brief Header file containing all definitions/functions related to dust sensor logger. 
*/

/**
* \defgroup dust_sensor DUST_SENSOR
* @{
*/
/** \enum dust_sensor_power_t Dust sensor's power settings */
typedef enum {
	/** When nor fan nor laser is powered */
	CG_DUST_SENSOR_POWER_OFF = 0,
	/** Bit to set fan power */
	CG_DUST_SENSOR_POWER_FAN_ON = 1,
	/** Bit to set laser power */
	CG_DUST_SENSOR_POWER_LASER_ON =  2,
	/** Max value dust sensor's power */
	CG_DUST_SENSOR_POWER_MAX = 3
} cg_dust_sensor_power_t;
/**
 * * \enum cg_dust_sensor_status_t The status of dust sensor's monitoring
 * */
typedef enum {
	/** When valid data is available */
	CG_DUST_SENSOR_VALID_DATA,
	/** When a read/write error occured */
	CG_DUST_SENSOR_RW_ERROR,
	/** when the dust sensor disconnects */
	CG_DUST_SENSOR_DISCONNECT,
	/** When the dust sensor connects */
	CG_DUST_SENSOR_CONNECT,
	/** Dust sensor not available */
	CG_DUST_SENSOR_NOT_AVAILABLE,
	/** Can't create thread */
	CG_DUST_SENSOR_THREAD_CREATE_ERROR,
	/** Invalid parameter */
	CG_DUST_SENSOR_INVALID_PARAMETER,
	/** Dust sensor is not ready */
	CG_DUST_SENSOR_NOT_AWAKE,
	/** Error setting spi mode */
	CG_DUST_SENSOR_SPI_ERROR
} cg_dust_sensor_status_t;

/**
* \struct cg_dust_sensor_data_t
* \brief Output parameters of the dust sensor logger
*/
/**
* \struct cg_dust_sensor_data_t
* \brief Structure describing logging interface
*/
typedef struct {
	/** 63 bytes raw logging data */
	unsigned char byte[63];
	/** 4 byte status number see_dust_sensor_status_t for meaning */
	uint32_t status;
} cg_dust_sensor_data_t;

/** Callback for reporting dust sensor data
* \param[in] dust sensor data
* \param[in] context Context as set in \ref cg_dust_sensor_register_data_callback
*/
typedef void (*cg_dust_sensor_data_cb_t)(const cg_dust_sensor_data_t *data, void *context);

/** Initiate dust sensor's logging.
* The function sets the supplied parameters and starts dust sensor logging with a given time interval.
* \param[in] time_interval value in seconds.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter,
* CG_STATUS_RESOURCE_BUSY in case no dust sensor found or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_dust_sensor_start(uint32_t time_interval);

/** Stop dust sensor monitoring
* \return CG_STATUS_OK if successful or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_dust_sensor_stop(void);

/** Sets dust sensor's power
* The function set the fan power and/or the laser power.
* \param[in] power 0 power off, 1 fan power on, 2 laser power on, 3 laser and fan power on
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter,
* CG_STATUS_RESOURCE_BUSY in case no dust sensor found or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_dust_sensor_set_power(uint32_t power);

/** Register a callback for dust_sensor data. It's possible to register multiple callbacks as long as
* either the callback or the context is different.
* \param[in] cb Callback for data.
* \param[in] context Context data passed to cb when called.
* \return CG_STATUS_OK if successful. CG_STATUS_INVALID_PARAMETER if no callback has been provided or CG_STATUS_ERROR if failure.
*/
cg_status_t cg_dust_sensor_register_data_callback(cg_dust_sensor_data_cb_t cb, void *context);

/** Deregister a previously registered callback
* \param[in] cb Callback to be called when logger data is received
* \param[in] context Context data which was passed on to the register call
* \return CG_STATUS_OK if successful. CG_STATUS_INVALID_PARAMETER if no matching callback and context has been found. CG_STATUS_ERROR (unknown error)
*/
cg_status_t cg_dust_sensor_deregister_data_callback(cg_dust_sensor_data_cb_t cb, void *context);


/** @} */

CG_END_DECLS

#endif /* __CG_DUST_SENSOR_H__ */
