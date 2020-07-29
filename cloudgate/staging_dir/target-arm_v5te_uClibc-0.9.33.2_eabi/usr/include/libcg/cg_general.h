#ifndef __CG_GENERAL_H__
#define __CG_GENERAL_H__
#include <time.h>
#include <libcg/cgate.h>
#include <sys/time.h>

CG_BEGIN_DECLS

/**
* \file cg_general.h
* \brief Header file containing definitions for miscellaneous functions
*/

/**
* \defgroup general General functions
* @{
*/

/** Current API level */
#define CG_SDK_API_LEVEL 01

/** Maximum length of a slot info string */
#define CG_MAX_SLOT_INFO_LEN 32

/* Maximum length of a slot uid string */
#define CG_MAX_SLOT_UID_LEN 64

/* Maximum length of a key/value namespace string, including terminating zero */
#define CG_KVPAIR_MAX_NS_LEN 64

/* Maximum length of a key/value key string, including terminating zero */
#define CG_KVPAIR_MAX_KEY_LEN 64

/* Maximum length of a key/value value blob, no termination */
#define CG_KVPAIR_MAX_VALUE_LEN 4096

/**
* \enum cg_prov_status_t
* \brief Provisioning status
*/
typedef enum {
	/** Device is currently not provisioned */
	CG_PROV_UNPROVISIONED,
	/** Device is checking for provisioning data */
	CG_PROV_CHECKING,
	/** Device is flashing new software */
	CG_PROV_FLASHING,
	/** Last provisioning attempt returned an error */
	CG_PROV_ERROR,
	/** Device is provisioned */
	CG_PROV_OK,
	/** Device is unregistered/not activated */
	CG_PROV_UNREGISTERED,
} cg_prov_status_t;

/**o
* \enum cg_slot_type_t
* \brief Different firmware slots
*/
typedef enum {
	/** Bootloader type */
	CG_SLOT_BOOTLOADER,
	/** Base image type (kernel + root filesystem) */
	CG_SLOT_BASE_IMAGE,
	/** Base configuration */
	CG_SLOT_BASE_CONFIG,
	/** Customer/developer firmware */
	CG_SLOT_CUSTOMER,
	/** Customer/developer configuration */
	CG_SLOT_CUSTOMER_CONFIG,
	/** Customer/developer public key */
	CG_SLOT_CUSTOMER_KEY
} cg_slot_type_t;

/**
 * \enum cg_dev_feature_t
 * \brief Various development features
 */
typedef enum {
	/** Root access */
	CG_DEV_ROOT_ACCESS,
	/** Set the minivan debug level */
	CG_DEV_MINIVAN_DEBUG
} cg_dev_feature_t;

/**
* \struct cg_slot_t
* \brief Device firmware slot description
*/
typedef struct {
	/** Type of slot */
	cg_slot_type_t type;
	/** Flag to indicate if this slot is currently active or not */
	uint8_t active;
	/** Size of the slot (in kilobytes) */
	uint32_t size;
	/** Version of the current image in that slot */
	char version[CG_MAX_SLOT_INFO_LEN];
	/** Unique ID of the slot */
	char uid[CG_MAX_SLOT_UID_LEN + 1];
} cg_slot_t;

/**
* \struct cg_slot_list_t
* \brief List of present slots
*/
typedef struct {
	/** Number of slots in *slots* */
	uint16_t num_slots;
	/** Array of slots */
	cg_slot_t slots[0];
} cg_slot_list_t;

/**
* \enum cg_log_level_t
*
*/
typedef enum {
	CG_LOG_DEBUG = 0x1,
	CG_LOG_INFO = 0x2,
	CG_LOG_WARN = 0x4,
	CG_LOG_ERR = 0x8
} cg_log_level_t;

/**
* \enum cg_system_boot_condition_t
* \brief System boot condition
*/
typedef enum {
	/** if this maskbit is set the system will boot when the ignition sense is detected */
	CG_SYSTEM_ARM_IGNITION_SENSE = 0x1,
	/** if this maskbit is set the system will boot when the "timed_wakeup" time has elapsed */
	CG_SYSTEM_TIMED_WAKEUP       = 0x2,
} cg_system_boot_condition_t;

/**
* \struct cg_kvpair_t
* \brief Key/value pair
* \remark The 'com.option.*' namespace is reserved for use by Option NV.
*/
typedef struct {
	/* Namespace */
	char ns[CG_KVPAIR_MAX_NS_LEN];
	/* Key */
	char key[CG_KVPAIR_MAX_KEY_LEN];
	/* Length of the value blob in bytes */
	int value_len;
	/* Value blob */
	uint8_t value[CG_KVPAIR_MAX_VALUE_LEN];
} cg_kvpair_t;

/**
* \struct cg_key_t
* \brief A single key of a key/value pair.
*/
typedef struct {
	char key[CG_KVPAIR_MAX_KEY_LEN];
} cg_key_t;

/**
* \struct cg_timezone_t
* \brief Time zone splitted up in its components.
* \ref https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
*/
typedef struct {
	/*  UTC offset : add to UTC to get the local time,
	 *  i.e. negative values are to the west of the Prime Meridian
	 *  and positive values are to the east of the Prime Meridian
	 */
	/* hours from -12 to +14 */
	int   utc_offset_h;
	/* minutes 0, 15, 30 and 45 are possible */
	int   utc_offset_m;
	/*  Daylight Saving Time entries
	 *  1 <= m <= 12, if m = 0 -> no DST is assumed for specific zone
	 *  1 <= n <=  5, where 5 means "the last <d> day in month <m>"
	 *                which may occur in either the 4th or the 5th week
	 *  0 <= d <=  6, where 0 means Sunday
	 */
	 /* month in which Standard Time changes to DST */
	int   dst_start_m;
	/* week <n> on of month <m> */
	int   dst_start_n;
	/* day <d> of week <n> */
	int   dst_start_d;
	/* month in which DST changes to Standard Time */
	int   dst_end_m;
	/* week <n> on of month <m> */
	int   dst_end_n;
	/* day <d> of week <n> */
	int   dst_end_d;
	/* Zone description */
	char desc[256];
} cg_timezone_t;

/**
* \enum cg_time_format_t
*
*/
typedef enum {
	CG_LOCAL_TIME,
	CG_UTC_TIME
} cg_time_format_t;

/**
* \enum cg_system_sync_ntp_state_t
*
*/
typedef enum {
	CG_NTP_SYNC_SUCCESS,
	CG_NTP_SYNC_FAILURE,
	CG_NTP_SYNC_NO_SERVER,
} cg_system_sync_ntp_state_t;

/**
* \struct cg_date_time_t
* \brief Structure holding local or UTC time value
*/
typedef struct {
	/* Structure containing a calendar date and time broken down into its components. */
	struct tm date_time;
	/* Specifies local time or UTC time format */
	cg_time_format_t time_format;
} cg_date_time_t;

/** Callback for key/value pair updates
* \param[in] context Context as set in \ref cg_prov_kvpair_register_update_callback
* \param[in] update Pointer to a \ref cg_kvpair_t containing the new/updated or deleted key/value pair.
* If deleted, value_len is 0. This \ref cg_kvpair_t must be free()d by the user-supplied callback.
*/
typedef void (*cg_prov_kvpair_update_cb_t)(cg_kvpair_t *update, void *context);

/** Callback for status code ntp sync
* \param[in] state The updated state.
* \param[in] context The context which was passed on in cg_system_sync_ntp call
*/
typedef void (*cg_system_sync_ntp_cb_t)( cg_system_sync_ntp_state_t state, void *context);

/** Initialize the SDK.
* \param name The name of the program/process that wants to use the SDK
* \return CG_STATUS_OK if successful or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_init(const char *name);

/** Uninitialize the SDK.
*/
cg_status_t cg_deinit(void);

/** Get the API level that the device supports. A user will need to compare this
* with the SDK API level and make a decision to go ahead or not.
* \param[out] level Integer indicating the API version
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER when provided
* with an invalid parameter or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_get_api_level(int32_t *level);

/** Get the serial number of the device.
* \param[out] serial_number When succesful, will be set to the serial number of the device,
* allocated by the callee. The caller will be responsible for freeing the allocated memory.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER when provided
* with an invalid parameter or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_get_serial_number(char **serial_number);

/** Get the current provisioning status
* \param[out] status Provisioning status
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER when provided
* with an invalid parameter or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_prov_get_status(cg_prov_status_t *status);

/** Trigger a check for new provisioning. The device will contact the provisioning server.
* \param[in] force Setting this flag to TRUE will force the device to re-provisioning
* regardless if it already has the correct firmware.
* \return CG_STATUS_OK if successful or CG_STATUS_ERROR upon error.
* \return CG_STATUS_INVALID_PARAMETER if supplied parameter is invalid.
*/
cg_status_t cg_prov_check_update(uint8_t force);

/** Enables or disables automatic provisioning by the provisioning server.
* \param[in] onoff Setting this flag to TRUE will enable automatic provisioning
* \return CG_STATUS_OK if successful or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_prov_set_automatic(uint32_t onoff);

/** Get the value corresponding to the key from the key/value passing system.
* \param[in] request_in A cg_kvpair_t with the key and ns members filled in.
* \param[out] If successful, the key, ns, value_len and value fields
* are filled in in a cg_kvpair_t structure allocated by callee.
* Caller is responsible for freeing the allocated memory.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER if the ns/key
* (or combination) is empty. On any error, CG_STATUS_ERROR is returned.
*/
cg_status_t cg_prov_kvpair_get(cg_kvpair_t *request_in, cg_kvpair_t **request_out);

/** Set the value corresponding to the key in the key/value passing system.
* \param[in] request A cg_kvpair_t with the key, ns, value and value_len members filled in,
* value_len must be positive.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER if the kvpair
* (or parts of it) is empty. On any error, CG_STATUS_ERROR is returned.
* If CG_STATUS_RESOURCE_UNAVAILABLE is returned, the caller must retry at a later
* time, since a sync is in progress with the CloudGate Universe.
* The supplied cg_kvpair_t is not freed, that is the caller's responsibility.
*/
cg_status_t cg_prov_kvpair_set(cg_kvpair_t *request);

/** Delete the entry corresponding to the key from the key/value passing system.
* \param[in] request A cg_kvpair_t with the key and ns members filled in,
* value and value_len are ignored.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER if the ns/key
* (or combination) is empty. On any error, CG_STATUS_ERROR is returned.
* If CG_STATUS_RESOURCE_UNAVAILABLE is returned, the caller must retry at a later
* time, since a sync is in progress with the CloudGate Universe.
* We treat the cg_kvpair_t as readonly, caller is responsible for freeing.
*/
cg_status_t cg_prov_kvpair_del(cg_kvpair_t *request);

/** Get a list of keys currently known in a given namespace.
* \param[in] ns The namespace in which to look for keys.
* \param[out] keys Will contain a pointer to a list of cg_key_t, caller must free.
* \param[out] n_keys Will contain the number of keys.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER if any of the
* parameters are 0 or malformed (ns too long, ...). CG_STATUS_RESOURCE_UNAVAILABLE
* if the key/value store is currently unavailable (f.e. being synced). CG_STATUS_ERROR
* on misc errors. The keys parameter will only contain something allocated on CG_STATUS_OK.
*/
cg_status_t cg_prov_kvpair_list(char *ns, int *n_keys, cg_key_t **keys);

/** Register a callback for key/value updates.
* \param[in] cb A cg_prov_kvpair_update_cb_t callback.
* \param[in] context Context data passed to the callback (untouched).
* \param[in] ns The namespace to watch for updates.
* \return CG_STATUS_OK if successful. CG_STATUS_INVALID_PARAMETER if no cb provided.
* CG_STATUS_ERROR for unspecified errors.
*/
cg_status_t cg_prov_kvpair_register_update_callback(cg_prov_kvpair_update_cb_t cb, void *context, const char *ns);

/** Deregister a previously registered callback
* \param[in] cb Callback (previously registered) to remove
* \param[in] context Context pointer which was previously registered
* \param[in] ns The namespace being watched by the callback to deregister.
* \return CG_STATUS_OK if succesful. CG_STATUS_INVALID_PARAMETER if no matching callback/context combination
* was found to remove. CG_STATUS_ERROR for unspecified errors.
*/
cg_status_t cg_prov_kvpair_deregister_update_callback(cg_prov_kvpair_update_cb_t cb, void *context, const char *ns);

/** Get the list of slots in the device
* \param[out] slot List of slots. Caller will need to free *slot.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER when provided
* with an invalid parameter or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_get_slot_list(cg_slot_list_t **slot);

/** Request a device reset. Optionally specify if this reset should include a factory restore or not.
* \param[in] fact_restore Request (0): a normal reset or (1): a factory restore which is done during the reboot.
* \param[in] reason Specify a reason for the reset. This reason will be logged.
* \return CG_STATUS_OK if successful or CG_STATUS_ERROR upon error. If the device is 
* unable to comply at that moment, CG_STATUS_RESOURCE_BUSY will be returned.
*/
cg_status_t cg_reset_system(uint8_t fact_restore, const char *reason);

/** Power cycle all peripherals.
* \return CG_STATUS_OK if successful, CG_STATUS_RESOURCE_BUSY if the device is unable to comply
* or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_reset_peripherals(void);

/** Log a line to the system log
* \param[in] level Log level 
* \param[in] format Log format
* \return CG_STATUS_OK
*/
cg_status_t cg_system_log(uint16_t log_level, const char *format,...) CG_GNUC_PRINTF(2,3);

/** Enable/disable a development feature
* \param[in] enable If true, the feature is enabled, if false, the feature is disabled again
* \param[in] feature the feature to enable/disable
* \param[in] options various configuration options of the different development features:
* char* requested_root_passwd in case of CG_DEV_ROOT_ACCESS, int level in case of CG_DEV_MINIVAN_DEBUG, ...
* \return CG_STATUS_OK on success, CG_STATUS_ERROR upon error, CG_STATUS_INVALID_PARAMETER
* when feature is not supported or out of range
*/
cg_status_t cg_set_dev_mode(uint8_t enable, cg_dev_feature_t feature, void *options);

/** Power down the complete system.
*  If the bit CG_SYSTEM_ARM_IGNITION_SENSE of the parameter system_boot_condition is set the system
*  will power down at function call. When the ignition sense line is armed the system will boot.
*  In case the mask bit is not set, the system will reboot at function call 
*  If the bit CG_SYSTEM_TIMED_WAKEUP of the parameter system_boot_condition is set the system will
*  power down at function call. When the "timed_wakeup" time has elapsed the system will wake up.
* \param[in] system_boot_condition : The condition needed to initiate system boot
* \return CG_STATUS_OK on success, CG_STATUS_ERROR upon error, CG_STATUS_INVALID_PARAMETER
* if parameter is invalid or CG_STATUS_RESOURCE_BUSY if already called
*/
cg_status_t cg_system_power_down (const cg_system_boot_condition_t system_boot_condition);
/** Sets the time in seconds to wake up the Cloudgate after a shutdown
* \param[in] seconds : Time specified in seconds
* \return CG_STATUS_OK on success, CG_STATUS_INVALID_PARAMETER
* if parameter is invalid and CG_STATUS_ERROR upon error.
*/
cg_status_t cg_system_set_timed_wakeup (uint32_t seconds);

/** Sets Calendar date and time
* \param[in] date_time: Holds calendar date and time, format can be local or UTC.
* Calendar date and time components are declared in "struct tm" (see "time.h" for details).
* \return CG_STATUS_OK on success, CG_STATUS_ERROR upon error or CG_STATUS_INVALID_PARAMETER
* if paramater is invalid
*/
cg_status_t cg_system_set_date_time(cg_date_time_t *date_time);

/** Retrieves Calender date and time
* \param[in] time_format : Specifies local time or UTC time.
* \param[out] date_time: Holds calendar date and time, format can be local or UTC.
* Parameter is allocated by the callee. The caller will be responsible for freeing the allocated memory.
* Calendar date and time components are declared in "struct tm" (see "time.h" for details).
* \return CG_STATUS_OK on success, CG_STATUS_ERROR upon error, CG_STATUS_INVALID_PARAMETER
* if paramater is invalid
*/
cg_status_t cg_system_get_date_time(cg_time_format_t time_format, cg_date_time_t **date_time);

/** Sets time zone
 * \param[in] timezone: at least utc_offset_h, utc_offset_m and desc must be filled in
 * \return CG_STATUS_OK on success, CG_STATUS_ERROR upon error or CG_STATUS_INVALID_PARAMETER
 * if paramater is invalid
 */
cg_status_t cg_system_set_timezone(cg_timezone_t *timezone);

/** Sets NTP server url
* \param[in] ntp_server_url: is a fully qualified domain name.
* \return CG_STATUS_OK on success, CG_STATUS_ERROR upon error or CG_STATUS_INVALID_PARAMETER
* if paramater is invalid
*/
cg_status_t cg_system_set_ntp_server(char *ntp_server_url);

/** Retrieves NTP server url
* \param[out] ntp_server_url: Is a fully qualified domain name and allocated by the callee.
* The caller will be responsible for freeing the allocated memory.
* \return CG_STATUS_OK on success, CG_STATUS_ERROR upon error, CG_STATUS_INVALID_PARAMETER
* if paramater is invalid
*/
cg_status_t cg_system_get_ntp_server(char **ntp_server_url);

/** Synchronizes time with Network Time Server and sets Real Time Clock if present
* \return CG_STATUS_OK on success or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_system_sync_ntp(void);

/** Registers callback for ntp synchronisation
* \param[in] callback : called when ntp synchronisation status changes
* \param[in] context Context parameter passed on to the callback when called.
* \return CG_STATUS_OK on success, CG_STATUS_INVALID_PARAMETER if parameter is invalid,
* or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_system_sync_ntp_register_callback(cg_system_sync_ntp_cb_t cb, void *context);

/** Deregisters callback for ntp synchronisation
* \param[in] callback : called when ntp synchronisation status changes
* \param[in] context Context parameter passed on to the callback when called.
* \return CG_STATUS_OK on success, CG_STATUS_INVALID_PARAMETER if parameter is invalid,
* or CG_STATUS_ERROR upon error.
*/
cg_status_t cg_system_sync_ntp_deregister_callback(cg_system_sync_ntp_cb_t cb, void *context);

/** @} */

CG_END_DECLS

#endif /* __CG_GENERAL_H__ */
