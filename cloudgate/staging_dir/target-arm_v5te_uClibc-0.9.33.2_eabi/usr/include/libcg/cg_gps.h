#ifndef __CG_GPS_H__
#define __CG_GPS_H__

#include <libcg/cgate.h>
#include <libcg/cg_net.h>

CG_BEGIN_DECLS

/**
* \file cg_gps.h
* \brief Header file containing definitions for all GPS-related functionality
*/

/**
* \defgroup gps GPS functions
* @{
*/

/**
* \note
* These two methods can be used to receive GPS NMEA data:
* - Using the GPS SDK NMEA callback (cg_gps_nmea_cb_t):  multiple applications can receive simultaneous GPS NMEA data
* - Using the GPS serial port ttyUSB2 after claiming the GPS port with cg_device_claim().
*
*/

/** The maximum number of certificates that can be written */
#define CG_MAX_CERTIFICATES 5

/** The maximum length of one certificate */
#define CG_MAX_LEN_CERTIFICATE 2000

/** Callback for reporting NMEA data
* \param[in] context Context as set in \ref cg_gps_register_nmea_callback
* \param[in] nmea_data NMEA character data
*/
typedef void (*cg_gps_nmea_cb_t)(const char *nmea_data, void *context);

/**
* \enum cg_gps_status_t
* \brief bitfield describing various status-flags for GPS
*/
typedef enum {
	/** Flag indicating if GPS functionality is physically present on the device */
	CG_GPS_PRESENT = 0x1,
	/** Flag indicating if GPS functionality is currently enabled */
	CG_GPS_ENABLED = 0x2
} cg_gps_status_t;

/**
* \enum cg_gps_assisted_connection_status_t
* \brief latest SUPL server connection indication
*/
typedef enum {
	/** Indication that the SUPL server connection passed and that data was successfully retrieved */
	CG_GPS_ASSISTED_CONNECTION_SUCCESS,
	/** Indication that the SUPL server connection was started and an error occurred */
	CG_GPS_ASSISTED_CONNECTION_FAILURE,
} cg_gps_assisted_connection_status_t;

/** Callback for reporting assisted GPS SUPL server connection status
* \param[in] connection status
*/
typedef void (*cg_gps_assisted_cb_t)(cg_gps_assisted_connection_status_t status);

/**
* \enum cg_gps_assisted_function_t
* \brief disable or enable AGPS with a secure or non secure connection to the SUPL server.
*/
typedef enum {
	/** disable AGPS: GPS will work standalone */
	CG_GPS_ASSISTED_DISABLE,
	/** enable AGPS non secure connection to SUPL server. */
	CG_GPS_ASSISTED_NON_SECURE_SUPL,
	/** enable AGPS. A secure connection will be made to the SUPL server. (root certificate must be provided) */
	CG_GPS_ASSISTED_SECURE_SUPL,
} cg_gps_assisted_function_t;

/**
* \struct cg_gps_t
* \brief Structure containing all GPS properties
*/
typedef struct {
	/** Status fields containing \ref cg_gps_status_t flags */
	uint8_t status;
	/** Reporting interval for GPS data */
	uint32_t reporting_interval;
} cg_gps_t;

/**
* \struct cg_gps_certificate_t
* \brief root certificate to validate the SUPL server.
*/
typedef struct {
	/** root certificate container
	* \note
	* each time new certificates are written the existing certificates will be deleted
	*/
	/** length of each certificate */
	uint32_t len;
	/** certificate in DER format */
	uint8_t cert[CG_MAX_LEN_CERTIFICATE];
} cg_gps_certificate_t;

/** Returns the current GPS status
* Note: even if GPS functionality is present, it could still be disabled.
* \param[out] gps Callee-allocated structure which is filled in with the GPS parameters.
* \return CG_STATUS_OK.
*/
cg_status_t cg_gps_get_status(cg_gps_t **gps);


/** Enables or disables GPS functionality
* \param[in] enabled Set to TRUE to enable GPS. Set to FALSE to disable GPS.
* \return CG_STATUS_OK if setting was successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter, CG_STATUS_ERROR (failure or no GPS device present)
* \note The CloudGate 3G rev 3 does not allow combined diversity and GPS. If diversity is enabled then this function will return CG_STATUS_ERROR.
*/
cg_status_t  cg_gps_set_enabled(int enabled);

/** Set GPS data reporting interval
* \param[in] interval Reporting interval in seconds
* \return CG_STATUS_OK if setting was successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter, CG_STATUS_ERROR (failure or no GPS device present)
*/
cg_status_t cg_gps_set_reporting_interval(uint32_t interval);

/** Set AGPS connection parameters
* \param[in] function: disable, enable secure or enable unsecure.
* \param[in] url: URL from SUPL server.
* \param[i] cert_count: number of root certificates that follow (maximum CG_MAX_CERTIFICATES)
* \param[in] certificates: for validating the SUPL server certificate (CG_GPS_ASSISTED_SECURE_SUPL only).
* \return CG_STATUS_OK if setting was successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter, CG_STATUS_ERROR (failure or no GPS device present)
* \note The SUPL server will only be conntacted if a valid PIN and APN is present, the device is registered, the GPS data is invalid and needs updating.
* \note This function will first stop GPS tracking (if already tracking) and then store the settings. Stopping GPS tracking could take about 60 seconds! It is therefore recommended to init agps before calling cg_gps_set_enabled(TRUE).
* \warning Assited gps will not respect the \"don't connect when roaming\" setting.
*
* example using google suple server
* secure url:   "supl.google.com:7275"
* unsecure url: "supl.google.com:7276"
*/
cg_status_t cg_gps_set_assisted(cg_gps_assisted_function_t function, const char *url, uint32_t cert_count, const cg_gps_certificate_t *cert);

/** Get AGPS connection parameters
* \param[out] pointer to function that will be set to: disable, enable secure or enable unsecure.
* \param[out] gps Callee-allocated string with supl addr URL when ASSISTED GPS is enabled
* \return CG_STATUS_OK if setting was successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter, CG_STATUS_ERROR (failure or no GPS device present)
* \note certificates can not be read from the gps device.
*/
cg_status_t cg_gps_get_assisted(cg_gps_assisted_function_t *function, char **url);

/** Register a callback for NMEA data. It's possible to register multiple callbacks as long as
* either the callback or the context is different.
* \param[in] cb Callback for NMEA data.
* \param[in] context Context data passed to cb when called.
* \return CG_STATUS_OK if successful. CG_STATUS_INVALID_PARAMETER if no callback has been provided. CG_STATUS_ERROR (failure or no GPS device present)
*/
cg_status_t cg_gps_register_nmea_callback(cg_gps_nmea_cb_t cb, void *context);

/** Deregister a previously registered callback
* \param[in] cb Callback to be called when NMEA data is received
* \param[in] context Context data which was passed on to the register call
* \return CG_STATUS_OK if successful. CG_STATUS_INVALID_PARAMETER if no matching callback and context has been found. CG_STATUS_ERROR (unknown error)
*/
cg_status_t cg_gps_deregister_nmea_callback(cg_gps_nmea_cb_t cb, void *context);

/** Register a callback for SUPL server connection status. Only one callback can be registered and must be cleared afterwards.
* \param[in] cb Callback for SUPL connection status.
* \return CG_STATUS_OK if successful. CG_STATUS_INVALID_PARAMETER if no callback has been provided. CG_STATUS_ERROR (failure or no GPS device present)
*/
cg_status_t cg_gps_register_supl_connection_status_callback(cg_gps_assisted_cb_t cb);

/** Deregister a previously registered callback
* \return CG_STATUS_OK if successful. CG_STATUS_ERROR (unknown error)
*/
cg_status_t cg_gps_deregister_supl_connection_status_callback(void);

/** @} */

CG_END_DECLS

#endif /* __CG_GPS_H__*/
