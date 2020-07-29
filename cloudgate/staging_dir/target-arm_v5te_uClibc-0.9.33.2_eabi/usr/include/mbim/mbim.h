#ifndef MBIM_H
#define MBIM_H

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <log/log.h>

/**
* \file mbim.h
* \brief header file which describes general public library functions
*/

/**
* \defgroup general General MBIM functions (PUBLIC)
* @{
*/

//Uncomment next line to enable more debugging.
//#define DEBUG

/** Log function */
#define libmbim_log(x, y, ...) log(LOG_LIBMBIM, x, y, ##__VA_ARGS__)

//TODO: This should not be a constant, but read from the device descriptor wMaxControlMessage.
/** Maximum control transfer */
#define MAX_CONTROL_TRANSFER 512
/** The maxiumum time between two fragments in milliseconds */
#define SEND_RECEIVE_TIMEOUT 1250

/**
 * \enum mbim_result_t
 * \brief return values after an mbim function is executed.
 */
typedef enum {
	/** The function executed successfully */
	MBIM_OK,
	/** An invalid parameter was provided to the function */
	MBIM_INVALID_PARAMETER,
	/** The function encountered an error */
	MBIM_ERROR,
	/** Buffer overflow (this one is not implemented yet) */
	MBIM_BUFFER_OVERFLOW,
	/** Internally used result code when an event is not expected */
	MBIM_EVENT_ERROR
} mbim_result_t;

/**
 * \struct uuid_t
 * \brief Universally Unique Identifier (UUID)
 */
typedef struct _uuid_t uuid_t;
#define MBIM_PACKET __attribute__((packed))
/**
 * \struct _uuid_t
 * \brief Packet struct for the Unique Universal Identifier.
 */
struct MBIM_PACKET _uuid_t {
	uint8_t a[4];
	uint8_t b[2];
	uint8_t c[2];
	uint8_t d[2];
	uint8_t e[6];
};
#undef MBIM_PACKET

/* MBIM status codes as defined on p67 */
/**
 * \enum mbim_status_codes_t
 * \brief Status code return from the MBIM device.
 */
typedef enum {
	MBIM_STATUS_SUCCESS,					/* 0 */
	MBIM_STATUS_BUSY,
	MBIM_STATUS_FAILURE,
	MBIM_STATUS_SIM_NOT_INSERTED,
	MBIM_STATUS_BAD_SIM,
	MBIM_STATUS_PIN_REQUIRED,				/* 5 */
	MBIM_STATUS_PIN_DISABLED,
	MBIM_STATUS_NOT_REGISTERED,
	MBIM_STATUS_PROVIDERS_NOT_FOUND,
	MBIM_STATUS_NO_DEVICE_SUPPORT,
	MBIM_STATUS_PROVIDER_NOT_VISIBLE, 		/* 10 */
	MBIM_STATUS_DATA_CLASS_NOT_AVAILABLE,
	MBIM_STATUS_PACKET_SERVICE_DETACHED,
	MBIM_STATUS_MAX_ACTIVATED_CONTEXTS,
	MBIM_STATUS_NOT_INITIALIZED,
	MBIM_STATUS_VOICE_CALL_IN_PROGRESS,		/* 15 */
	MBIM_STATUS_CONTEXT_NOT_ACTIVATED,
	MBIM_STATUS_SERVICE_NOT_ACTIVATED,
	MBIM_STATUS_INVALID_ACCESS_STRING,
	MBIM_STATUS_INVALID_USER_NAME_PWD,
	MBIM_STATUS_RADIO_POWER_OFF,			/* 20 */
	MBIM_STATUS_INVALID_PARAMETERS,
	MBIM_STATUS_READ_FAILURE,
	MBIM_STATUS_WRITE_FAILURE,
	//Reserved, /*24*/
	MBIM_STATUS_NO_PHONEBOOK = 25,
	MBIM_STATUS_PARAMETER_TOO_LONG,
	MBIM_STATUS_STK_BUSY,
	MBIM_STATUS_OPERATION_NOT_ALLOWED,
	MBIM_STATUS_MEMORY_FAILURE,
	MBIM_STATUS_INVALID_MEMORY_INDEX,		/* 30 */
	MBIM_STATUS_MEMORY_FULL,
	MBIM_STATUS_FILTER_NOT_SUPPORTED,
	MBIM_STATUS_DSS_INSTANCE_LIMIT,
	MBIM_STATUS_INVALID_DEVICE_SERVICE_OPERATION,
	MBIM_STATUS_AUTH_INCORRECT_AUTN,		/* 35 */
	MBIM_STATUS_AUTH_SYNC_FAILURE,
	MBIM_STATUS_AUTH_AMF_NOT_SET,
	MBIM_STATUS_CONTEXT_NOT_SUPPORTED,
	MBIM_STATUS_SMS_UNKNOWN_SMSC_ADDRESS = 100,
	MBIM_STATUS_SMS_NETWORK_TIMEOUT,
	MBIM_STATUS_SMS_LANG_NOT_SUPPORTED,
	MBIM_STATUS_SMS_ENCODING_NOT_SUPPORTED,
	MBIM_STATUS_SMS_FORMAT_NOT_SUPPORTED
} mbim_status_codes_t;

/* Command ID (CID) types as defined on p75 */
/**
 * \enum mbim_cid_t
 * \brief The different command IDs which can be used to SET or QUERY the MBIM device.
 */
typedef enum {
	MBIM_CID_DEVICE_CAPS = 1,
	MBIM_CID_SUBSCRIBER_READY_STATUS,
	MBIM_CID_RADIO_STATE,
	MBIM_CID_PIN,
	MBIM_CID_PIN_LIST,
	MBIM_CID_HOME_PROVIDER,
	MBIM_CID_PREFERRED_PROVIDERS,
	MBIM_CID_VISIBLE_PROVIDERS,
	MBIM_CID_REGISTER_STATE,
	MBIM_CID_PACKET_SERVICE,
	MBIM_CID_SIGNAL_STATE,
	MBIM_CID_CONNECT,
	MBIM_CID_PROVISIONED_CONTEXTS,
	MBIM_CID_SERVICE_ACTIVATION,
	MBIM_CID_IP_CONFIGURATION,
	MBIM_CID_DEVICE_SERVICES,
	MBIM_CID_DEVICE_SERVICE_SUBSCRIBE_LIST = 19,
	MBIM_CID_PACKET_STATISTICS,
	MBIM_CID_NETWORK_IDLE_HINT,
	MBIM_CID_EMERGENCY_MODE,
	MBIM_CID_IP_PACKET_FILTERS,
	MBIM_CID_MULTICARRIER_PROVIDERS,
	MBIM_CID_SMS_CONFIGURATION = 1,
	MBIM_CID_SMS_READ,
	MBIM_CID_SMS_SEND,
	MBIM_CID_SMS_DELETE,
	MBIM_CID_SMS_MESSAGE_STORE_STATUS,
	MBIM_CID_USSD = 1,
	MBIM_CID_PHONEBOOK_CONFIGURATION = 1,
	MBIM_CID_PHONEBOOK_READ,
	MBIM_CID_PHONEBOOK_DELETE,
	MBIM_CID_PHONEBOOK_WRITE,
	MBIM_CID_STK_PAC = 1,
	MBIM_CID_STK_TERMINAL_RESPONSE,
	MBIM_CID_STK_ENVELOPE,
	MBIM_CID_AKA_AUTH = 1,
	MBIM_CID_AKAP_AUTH,
	MBIM_CID_SIM_AUTH,
	MBIM_CID_DSS_CONNECT = 1,
	MBIM_CID_THERMAL_STATE = 0x2000100,
	MBIM_CID_INTC_SAR_CONFIG = 1,
	MBIM_CID_INTC_MODEM_REBOOT = 1,
	MBIM_CID_MS_HOSTSHUTDOWN = 1,
	MBIM_CID_INTC_ADPCLK_MODE = 0x2000010,
	MBIM_CID_INTC_TRACE = 1,
	MBIM_CID_INTC_TRACE_DATA = 2,
	MBIM_CID_INTC_NRTAPP = 1,
	MBIM_CID_INTC_NRTCWS = 2,
	MBIM_CID_INTC_USBPROFILE = 1,
	MBIM_CID_INTC_CIQ = 1,
	MBIM_CID_ATDS_SIGNAL = 1,
	MBIM_CID_ATDS_LOCATION,
	MBIM_CID_ATDS_OPERATORS,
	MBIM_CID_ATDS_RAT,
	MBIM_CID_ATDS_REGISTER_STATE = 9,
	MBIM_CID_ATDS_RAT_DISPLAY = 11
} mbim_cid_t;

/* MBIM services and CID values as defined on p74 */
/**
 * \enum mbim_services_t
 * \brief The different services for the MBIM device
 */
typedef enum {
	MBIM_SERVICE_INVALID,
	MBIM_SERVICE_BASIC_CONNECT,
	MBIM_SERVICE_SMS,
	MBIM_SERVICE_USSD,
	MBIM_SERVICE_PHONEBOOK,
	MBIM_SERVICE_STK,
	MBIM_SERVICE_AUTH,
	MBIM_SERVICE_DSS,
	MBIM_SERVICE_INTEL_DPTF,
	MBIM_SERVICE_INTEL_FWUSVC,
	MBIM_SERVICE_INTEL_SAR,
	MBIM_SERVICE_MS_HOSTSHUTDOWN,
	MBIM_SERVICE_INTEL_ACT,
	/** Intel Trace service */
	MBIM_SERVICE_INTEL_TRCSVC,
	/** AT&T Extensions */
	MBIM_SERVICE_ATDS,
	MBIM_SERVICE_INTEL_NRTC,
	MBIM_SERVICE_INTEL_USB_PROFILE,
	MBIM_SERVICE_INTEL_CIQ
} mbim_services_t;

/**
 * Translate the mbim_services_t value to a string.
 * \param[in] service mbim_services_t value.
 * \return string value.
 */
const char* mbim_get_service_string(mbim_services_t service);

/**
 * Translate the mbim_cid_t value to a string.
 * \param[in[ service The MBIM service.
 * \param[in] cid The MBIM command id.
 * \return string value.
 */
const char* mbim_get_cid_string(mbim_services_t service, mbim_cid_t cid);

/**
 * \struct mbim_context_t
 * \brief The MBIM device context.
 */
typedef struct {
	/** Device name. I.e. /dev/cdc-wdm0 */
	char *dev_name;
	/** fd set after doing a mbim_init */
	int fd;
} mbim_context_t;

/* Callback function the client has to implement. I.e: to retrieve the pin info */
/**
 * Callback prototype when the client does a SET or QUERY.
 * \param[out] status The status code based on mbim_status_codes_t.
 * \param[out] data The returned  parsed data from the MBIM device.
 * \param[out] length The length of the returned data from the MBIM device.
 * \param[out] ctxt The context the client passed when doing a SET or QUERY command.
 */
typedef void (*mbim_user_data_cb)(uint32_t status, void *data, uint32_t length, void *ctx);

/**
 * The first function a client has to call when using the library. The library tries to
 * open the communication with the MBIM device and sets the filedescriptor.
 * \param[in] dev_name The device to open. I.e. /dev/cdc-wdm0.
 * \return NULL on error, otherwise a pointer to the mbim_context_t struct. The struct
 * contains a valid filedescriptor.
 * \warning The client should not change the parameters in the struct.
 */
mbim_context_t *mbim_init(const char *dev_name);

/**
 * Deinit the MBIM device. All open transactions are closed. All registered notifications
 * are deleted.
 * \param[in] ctx The MBIM device context.
 * \return MBIM_OK, MBIM_INVALID_PARAMETER, MBIM_ERROR.
 */
uint32_t mbim_deinit(mbim_context_t *ctx);

/* Notifications */
/*
 * Callback function the client has to implement to handle notifications.
 * Later, each command has its own callback.
 */

/** Callback function the client has to implement when registering for.
 * notifications.
 * \param[out] status The status code based on mbim_status_codes_t.
 * \param[out] data The returned  parsed data from the MBIM device.
 * \param[out] length The length of the returned data from the MBIM device.
 * \param[out] ctxt The context the client passed when doing a SET or QUERY command.
 */
typedef void (*mbim_notification_cb)(uint32_t status, void *data, uint32_t length, void *arg);

/**
 * Register for a specific unsolicited event (notification)
 * \param[in] ctx Pointer to the MBIM device context.
 * \param[in] service The MBIM service. I.e. MBIM_SERVICE_BASIC_CONNECT.
 * \param[in] command_id The MBIM command id. I.e. MBIM_CID_CONNECT.
 * \param[in] Client callback function to handle the notification.
 * \param[in] arg Extra argument for the client callback function.
 * \return MBIM_OK, MBIM_INVALID_PARAMETER, MBIM_ERROR
 */
uint32_t mbim_register_notification(mbim_context_t *ctx, mbim_services_t service, \
		mbim_cid_t command_id, mbim_notification_cb callback, void *arg);

/**
 * Deregister a specific unsolicited event (notification)
 * \param[in] ctx Pointer to the MBIM device context.
 * \param[in] service The MBIM service. I.e. MBIM_SERVICE_BASIC_CONNECT.
 * \param[in] command_id The MBIM command id. I.e. MBIM_CID_CONNECT.
 * \return MBIM_OK, MBIM_INVALID_PARAMETER, MBIM_ERROR
 */
uint32_t mbim_deregister_notification(mbim_context_t *ctx, mbim_services_t service, \
		mbim_cid_t command_id);

/** @} */

#endif
