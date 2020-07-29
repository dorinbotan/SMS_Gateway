#ifndef MBIM_BASIC_CONNECT_H
#define MBIM_BASIC_CONNECT_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <mbim/mbim_event.h>
#include <mbim/mbim.h>

/**
 * \file mbim_basic_connect.h
 * \brief Public header file which contains all command id functionality for the
 * basic connect services
 */

/**
 * \defgroup basic_connect_services BASIC CONNECT functions (PUBLIC)
 * @{
 */

/* MBIM_CID_PIN as defined on p94 */
typedef enum {
	MBIM_PIN_TYPE_NONE,
	MBIM_PIN_TYPE_CUSTOM,
	MBIM_PIN_TYPE_PIN_1,
	MBIM_PIN_TYPE_PIN_2,
	MBIM_PIN_TYPE_DEVICE_SIM_PIN,
	MBIM_PIN_TYPE_DEVICE_FIRST_SIM_PIN,
	MBIM_PIN_TYPE_NETWORK_PIN,
	MBIM_PIN_TYPE_NETWORK_SUBSET_PIN,
	MBIM_PIN_TYPE_SERVICE_PROVIDER_PIN,
	MBIM_PIN_TYPE_CORPORATE_PIN,
	MBIM_PIN_TYPE_SUBSIDY_LOCK,
	MBIM_PIN_TYPE_PUK_1,
	MBIM_PIN_TYPE_PUK_2,
	MBIM_PIN_TYPE_DEVICE_FIRST_SIM_PUK,
	MBIM_PIN_TYPE_NETWORK_PUK,
	MBIM_PIN_TYPE_NETWORK_SUBSET_PUK,
	MBIM_PIN_TYPE_SERVICE_PROVIDER_PUK,
	MBIM_PIN_TYPE_CORPORATE_PUK
} mbim_pin_type_t;

typedef enum {
	MBIM_PIN_STATE_UNLOCKED,
	MBIM_PIN_STATE_LOCKED
} mbim_pin_state_t;

typedef enum {
	MBIM_PIN_OPERATION_ENTER,
	MBIM_PIN_OPERATION_ENABLE,
	MBIM_PIN_OPERATION_DISABLE,
	MBIM_PIN_OPERATION_CHANGE
} mbim_pin_operation_t;

/* Pin struct to do some pin operations */
typedef struct {
	mbim_pin_type_t pin_type;
	mbim_pin_operation_t pin_operation;
	uint32_t pin_offset;
	uint32_t pin_size;
	uint32_t new_pin_offset;
	uint32_t new_pin_size;
	uint8_t data_buffer[];
} mbim_set_pin_t;

/* Pin struct response from device */
typedef struct {
	mbim_pin_type_t pin_type;
	mbim_pin_state_t pin_state;
	uint32_t remaining_attempts;
} mbim_pin_info_t;

/**
 * \enum mbim_context_type_t
 * \brief Specifies the type of context being represented
 */
typedef enum {
	MBIM_CONTEXT_TYPE_INVALID,
	MBIM_CONTEXT_TYPE_NONE,
	MBIM_CONTEXT_TYPE_INTERNET,
	MBIM_CONTEXT_TYPE_VPN,
	MBIM_CONTEXT_TYPE_VOICE,
	MBIM_CONTEXT_TYPE_VIDEO_SHARE,
	MBIM_CONTEXT_TYPE_PURCHASE,
	MBIM_CONTEXT_TYPE_IMS,
	MBIM_CONTEXT_TYPE_MMS,
	MBIM_CONTEXT_TYPE_LOCAL,
	MBIM_CONTEXT_TYPE_MAX
} mbim_context_type_t;

uint32_t mbim_get_pin_info(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);
uint32_t mbim_enter_puk(mbim_context_t *ctx, mbim_user_data_cb cb, \
		const char *puk, const char *new_pin ,mbim_pin_type_t pin_type ,void *arg);
uint32_t mbim_enter_pin(mbim_context_t *ctx, mbim_user_data_cb cb, \
		const char *pin, mbim_pin_type_t pin_type, void *arg);
uint32_t mbim_enable_pin(mbim_context_t *ctx, mbim_user_data_cb cb, \
		const char *pin, mbim_pin_type_t pin_type, void *arg);
uint32_t mbim_disable_pin(mbim_context_t *ctx, mbim_user_data_cb cb, \
		const char *pin, mbim_pin_type_t pin_type, void *arg);
uint32_t mbim_change_pin(mbim_context_t *ctx, mbim_user_data_cb cb, \
		const char *pin, const char *new_pin, mbim_pin_type_t pin_type, void *arg);

/* MBIM CID CONNECT as defined on p131 */

typedef enum {
	MBIM_ACTIVATION_COMMAND_DEACTIVATE,
	MBIM_ACTIVATION_COMMAND_ACTIVATE
} mbim_activation_command_t;

typedef enum {
	MBIM_COMPRESSION_NONE,
	MBIM_COMPRESSION_ENABLE,
	MBIM_COMPRESSION_MAX
} mbim_compression_t;

typedef enum {
	MBIM_AUTH_PROTOCOL_NONE,
	MBIM_AUTH_PROTOCOL_PAP,
	MBIM_AUTH_PROTOCOL_CHAP,
	MBIM_AUTH_PROTOCOL_MS_CHAP_V2
} mbim_auth_protocol_t;

typedef enum {
	MBIM_CONTEXT_IP_TYPE_DEFAULT,
	MBIM_CONTEXT_IP_TYPE_IPV4,
	MBIM_CONTEXT_IP_TYPE_IPV6,
	MBIM_CONTEXT_IP_TYPE_IPV4_V6,
	MBIM_CONTEXT_IP_TYPE_IPV4_AND_IPV6 /* TWO PDP CONTEXTS INTO ONE DATA STREAM */
} mbim_context_ip_type_t;

typedef enum {
	MBIM_ACTIVATION_STATE_UNKNOWN,
	MBIM_ACTIVATION_STATE_ACTIVATED,
	MBIM_ACTIVATION_STATE_ACTIVATING,
	MBIM_ACTIVATION_STATE_DEACTIVATED,
	MBIM_ACTIVATION_STATE_DEACTIVATING
} mbim_activation_state_t;

typedef enum {
	MBIM_VOICE_CALL_STATE_NONE,
	MBIM_VOICE_CALL_STATE_IN_PROGRESS,
	MBIM_VOICE_CALL_STATE_HANG_UP
} mbim_voice_call_state_t;

/* MBIM_CID_VISIBLE_PROVIDERS as defined on p109 */

typedef enum {
	MBIM_VISIBLE_PROVIDERS_ACTION_FULL_SCAN,
	MBIM_VISIBLE_PROVIDERS_ACTION_RESTRICTED_SCAN
} mbim_visible_providers_action_t;

typedef enum {
	MBIM_CELLULAR_CLASS_GSM=1,
	MBIM_CELLULAR_CLASS_CDMA
} mbim_cellular_class_t;

typedef enum {
	MBIM_PROVIDER_STATE_UNKNOWN,
	MBIM_PROVIDER_STATE_HOME,
	MBIM_PROVIDER_STATE_FORBIDDEN,
	MBIM_PROVIDER_STATE_PREFERRED=4,
	MBIM_PROVIDER_STATE_VISIBLE=8,
	MBIM_PROVIDER_STATE_REGISTERED=16,
	MBIM_PROVIDER_STATE_PREFERRED_MULTICARRIER=32
} mbim_provider_state_t;

/**
 *  \struct mbim_provider_t
 *  \brief Struct Provider in user format
 */
typedef struct {
	char *provider_id;
	mbim_provider_state_t provider_state;
	char *provider_name;
	mbim_cellular_class_t cellular_class;
	uint32_t rssi;
	uint32_t error_rate;
} mbim_provider_t;

/**
 **  \struct mbim_nw_list_t
 *  \brief Struct returned to user when visible providers are requested
 */

typedef struct {
	uint32_t count;
	mbim_provider_t providers[];
} mbim_nw_list_t;

/**
 *  \struct mbim_visible_providers_req_t
 *  \brief Struct used to request the visible providers
 */
typedef struct {
	/** action defines if a full scan or a restricted scan is required */
	mbim_visible_providers_action_t action;
} mbim_visible_providers_req_t;

/**
 * Request the visible providers
 * \param[in] ctx pointer to the context of the mbim device
 * \param[in] cb user callback to be called when the visible providers are received
 * \param[in] action defines the if a full scan or a restricted scan is required
 * \param[in] arg extra arguments.
 */
uint32_t mbim_get_providers(mbim_context_t *ctx, mbim_user_data_cb cb, mbim_visible_providers_action_t action, void *arg);

/**
 * Cancel all open visible providers scans.
 * \param[in] ctx pointer to the context of the mbim device
 * \return MBIM_OK, MBIM_INVALID_PARAMETER, MBIM_ERROR.
 */
uint32_t mbim_cancel_providers_scan(mbim_context_t *ctx);

/**
 * Request the home provider
 * \param[in] ctx pointer to the context of the mbim device
 * \param[in] cb user callback to be called when the visible providers are received
 * \param[in] arg extra arguments.
 */
uint32_t mbim_get_home_provider(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * Request the preferred providers
 * \param[in] ctx pointer to the context of the mbim device
 * \param[in] cb user callback to be called when the visible providers are received
 * \param[in] arg extra arguments.
 */
uint32_t mbim_get_preferred_providers(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * Set the preferred providers
 * \param[in] ctx pointer to the context of the mbim device
 * \param[in] cb user callback to be called when the visible providers are received
 * \param[in] providers List of providers of type mbim_nw_list_t
 * \param[in] arg extra arguments.
 */
uint32_t mbim_set_preferred_providers(mbim_context_t *ctx, mbim_user_data_cb cb,
		mbim_nw_list_t *providers, void *arg);

/* MBIM context types in mbim_context_type.h/c */

typedef struct {
	uint32_t session_id;
	mbim_activation_command_t activation_command;
	uint32_t access_string_offset;
	uint32_t access_string_size;
	uint32_t user_name_offset;
	uint32_t user_name_size;
	uint32_t password_offset;
	uint32_t password_size;
	mbim_compression_t compression;
	mbim_auth_protocol_t auth_protocol;
	mbim_context_ip_type_t ip_type;
	uuid_t context_type;
} mbim_set_connect_t __attribute__ ((aligned(4)));

typedef struct {
	uint32_t session_id;
	mbim_activation_state_t activation_state;
	mbim_voice_call_state_t voice_call_state;
	mbim_context_ip_type_t ip_type;
	uuid_t context_type;
	uint32_t nw_error; //Network specific error.
} mbim_connect_info_t;

/**
 * \struct mbim_connect_reply_t
 * \brief Response struct after parsing the incoming data from the MBIM device.
 * This struct is based on mbim_connect_info_t.
 */
typedef struct {
	uint32_t session_id;
	mbim_activation_state_t activation_state;
	mbim_voice_call_state_t voice_call_state;
	mbim_context_ip_type_t ip_type;
	mbim_context_type_t context_type;
	uint32_t nw_error;
} mbim_connect_reply_t;

/**
* \struct mbim_connect_details_t
* \brief User struct to set the connect details. Use this struct
* for mbim_connect.
*/
typedef struct {
	uint32_t session_id;
	char *apn;
	char *username;
	char *password;
	mbim_compression_t compression;
	mbim_auth_protocol_t auth_protocol;
	mbim_context_ip_type_t ip_type;
	mbim_context_type_t context_type;
} mbim_connect_details_t;

uint32_t mbim_connect(mbim_context_t *ctx, mbim_user_data_cb cb, \
		mbim_connect_details_t *connect_details, void *arg);

uint32_t mbim_get_connect_info(mbim_context_t *ctx, mbim_user_data_cb cb, \
		uint32_t session_id, void *arg);

uint32_t mbim_disconnect(mbim_context_t *ctx, mbim_user_data_cb,
		uint32_t session_id, void *arg);

/* MBIM CID IP CONFIGURATION as defined in MBIM spec v1.0 on p172 */

/**
 * \struct mbim_ipv4_addr_t
 * \brief Struct to represent an ipv4 address.
 */
typedef struct {
	/** IPv4 address */
	uint8_t ipv4[4];
} mbim_ipv4_addr_t;

/**
 * \struct mbim_ipv6_addr_t
 * \brief Struct to represent an ipv6 address.
 */
typedef struct {
	/** IPv6 address */
	uint8_t ipv6[16];
} mbim_ipv6_addr_t;

/**
 * \struct mbim_ipv4_element_t
 * \brief Struct to represent an ipv4 address with netmask.
 */
typedef struct {
	/** Netmask (OnLinkPrefixLength) */
	uint32_t netmask;
	/** Ipv4 address */
	mbim_ipv4_addr_t ip;
} mbim_ipv4_element_t;

/**
 * \struct mbim_ipv6_element_t
 * \brief Struct to represent an ipv6 address with netmask.
 */
typedef struct {
	/** Netmask (Onlinkprefixlength) */
	uint32_t netmask;
	/** Ipv6 address */
	mbim_ipv6_addr_t ip;
} mbim_ipv6_element_t;

/**
 * \struct mbim_ip_config_reply_t
 * \brief Response struct after parsing incoming data from the MBIM device.
 * This struct is used after an MBIM_CID_IP_CONFIGURATION command/notification.
 */
typedef struct {
	/** Session id */
	uint32_t session_id;
	/** Number of ipv4 addresses */
	uint32_t ipv4_count;
	/** Pointer to an array of ipv4 addresses */
	mbim_ipv4_element_t *ipv4;
	/** IPv4 gateway */
	mbim_ipv4_addr_t ipv4_gw;
	/** Number of ipv4 dns servers */
	uint32_t ipv4_dns_count;
	/** Pointer to an array of ipv4 dns servers */
	mbim_ipv4_addr_t *ipv4_dns;
	/** IPv4 mtu */
	uint32_t ipv4_mtu;
	/** Number of ipv6 addresses */
	uint32_t ipv6_count;
	/** Pointer to an array of ipv6 addresses */
	mbim_ipv6_element_t *ipv6;
	/** IPv6 gateway */
	mbim_ipv6_addr_t ipv6_gw;
	/** Number of ipv6 dns servers */
	uint32_t ipv6_dns_count;
	/** Pointer to an array of ipv6 dns servers */
	mbim_ipv6_addr_t *ipv6_dns;
	/** IPv6 mtu */
	uint32_t ipv6_mtu;
} mbim_ip_config_reply_t;

typedef struct {
	uint32_t session_id;
	uint8_t padding[56];
} mbim_ip_configuration_info_t;

/**
 * Do a query to get the ip configuration.
 * \param[in] ctx The mbim device context.
 * \param[in] user data callback
 * \param[in] arg Extra argument for the user callback.
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_ip_configuration(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/* MBIM CID PIN_LIST as defined on p97 */

/**
 * \enum mbim_pin_mode_t
 * \brief The pin mode: enabled, disabled, not supported.
 */
typedef enum {
	MBIM_PIN_MODE_NO_SUPPORTED,
	MBIM_PIN_MODE_ENABLED,
	MBIM_PIN_MODE_DISABLED
} mbim_pin_mode_t;

/**
 * \enum mbim_pin_format_t
 * \brief Format of the pin: unknown, numeric, alphanumeric.
 */
typedef enum {
	MBIM_PIN_FORMAT_UNKNOWN,
	MBIM_PIN_FORMAT_NUMERIC,
	MBIM_PIN_FORMAT_ALPHA_NUMERIC
} mbim_pin_format_t;

/**
 * \struct mbim_pin_desc_t
 * \brief Describing struct for a specific pin type.
 */
typedef struct {
	/** Lock enabled or not */
	mbim_pin_mode_t pin_mode;
	/** Pin format */
	mbim_pin_format_t pin_format;
	/** Minimum number of characters. Must be smaller than 17. 0xffffffff if not available */
	uint32_t pin_length_min;
	/** Maximum number of characters. Must be smaller than 17. 0xffffffff if not available */
	uint32_t pin_length_max;
} mbim_pin_desc_t;

/**
 * \struct mbim_pin_list_info_t
 * \brief Reply when query for cid pin list.
 */
typedef struct {
	mbim_pin_desc_t pin_desc_pin1;
	mbim_pin_desc_t pin_desc_pin2;
	mbim_pin_desc_t pin_desc_device_sim_pin;
	mbim_pin_desc_t pin_desc_device_first_pin;
	mbim_pin_desc_t pin_desc_network_pin;
	mbim_pin_desc_t pin_desc_network_subset_pin;
	mbim_pin_desc_t pin_desc_service_provider_pin;
	mbim_pin_desc_t pin_desc_corporate_pin;
	mbim_pin_desc_t pin_desc_subsidy_lock;
	mbim_pin_desc_t pin_desc_custom;
} mbim_pin_list_info_t;

/**
 * Request the pin list..
 * \param[in] ctx The mbim device context.
 * \param[in] user data callback
 * \param[in] arg Extra argument for the user callback.
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_pin_list(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/* MBIM CID IP RADIO STATE as defined in MBIM spec v1.0 on p92 */

/**
* \enum mbim_radio_switch_state_t
* \brief MBIM device's radio power state.
*/
typedef enum {
	MBIM_RADIO_OFF,
	MBIM_RADIO_ON
} mbim_radio_switch_state_t;

/**
* \struct mbim_set_radio_state_t
* \brief Struct used in the informationbuffer to enable/disable the MBIM device's radio.
**/
typedef struct {
	mbim_radio_switch_state_t radio_state;
} mbim_set_radio_state_t;

/**
* \struct mbim_radio_state_info_t
* \brief Response from the MBIM device when doing a set or query for the radio state.
**/
typedef struct {
	/** Hardware radio switch state */
	mbim_radio_switch_state_t hw_radio_state;
	/** Software radio switch state */
	mbim_radio_switch_state_t sw_radio_state;
} mbim_radio_state_info_t;

/**
 * Enable the software radio state
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_enable_radio(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * Disable the software radio state
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_disable_radio(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * Query the radio state. The data in the callback will be a mbim_radio_state_info_t struct.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_radio(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/* MBIM CID DEVICE CAPS as defined in MBIM spec v1.0 on p78 */

/**
 * \enum mbim_device_type_t
 * \brief Device types when querying for device capabilities.
 */
typedef enum {
	MBIM_DEVICE_TYPE_UNKNOWN,
	MBIM_DEVICE_TYPE_EMBEDDED,
	MBIM_DEVICE_TYPE_REMOVABLE,
	MBIM_DEVICE_TYPE_REMOTE
} mbim_device_type_t;

/**
 * \enum mbim_voice_class_t
 * \brief This member informs the host about the presence of circuit voice services.
 */
typedef enum {
	MBIM_VOICE_CLASS_UNKNOWN,
	MBIM_VOICE_CLASS_NO_VOICE,
	MBIM_VOICE_CLASS_SEPARATE_VOICE_DATA,
	MBIM_VOICE_CLASS_SIMULTANEOUS_VOICE_DATA
} mbim_voice_class_t;

/**
 * \enum mbim_sim_class_t
 * \brief Sim class
 */
typedef enum {
	MBIM_SIM_CLASS_SIM_LOGICAL,
	MBIM_SIM_CLASS_REMOVABLE
} mbim_sim_class_t;

/**
 * \enum mbim_data_class_t
 * \brief Supported radio technologies by the device.
 */
typedef enum {
	MBIM_DATA_CLASS_NONE,
	MBIM_DATA_CLASS_GPRS,
	MBIM_DATA_CLASS_EDGE,
	MBIM_DATA_CLASS_UMTS			= 0x4,
	MBIM_DATA_CLASS_HSDPA			= 0x8,
	MBIM_DATA_CLASS_HSUPA			= 0x10,
	MBIM_DATA_CLASS_LTE				= 0x20,
	MBIM_DATA_CLASS_1XRTT			= 0x10000,
	MBIM_DATA_CLASS_1XEVDO			= 0x20000,
	MBIM_DATA_CLASS_1XEVDO_REV_A	= 0x40000,
	MBIM_DATA_CLASS_1XEVDV			= 0x80000,
	MBIM_DATA_CLASS_3XRTT			= 0x100000,
	MBIM_DATA_CLASS_1XEVDO_REV_B	= 0x200000,
	MBIM_DATA_CLASS_UMB				= 0x400000,
	MBIM_DATA_CLASS_CUSTOM			= 0x80000000
} mbim_data_class_t;

/**
 * \enum mbim_sms_caps_t
 * \brief Type of SMS messages and directional flow.
 */
typedef enum {
	MBIM_SMS_CAPS_NONE,
	MBIM_SMS_CAPS_PDU_RECEIVE,
	MBIM_SMS_CAPS_PDU_SEND,
	MBIM_SMS_CAPS_TEXT_RECEIVE = 4,
	MBIM_SMS_CAPS_TEXT_SEND = 8
} mbim_sms_caps_t;

/**
 * \enum mbim_ctrl_caps_t
 * \brief Represents the control capabilities that the device supports.
 */
typedef enum {
	MBIM_CTRL_CAPS_NONE,
	MBIM_CTRL_CAPS_REG_MANUAL,
	MBIM_CTRL_CAPS_HW_RADIO_SWITCH,
	MBIM_CTRL_CAPS_CDMA_MOBILE_IP	= 0x4,
	MBIM_CTRL_CAPS_CDMA_SIMPLE_IP	= 0x8,
	MBIM_CTRL_CAPS_MULTI_CARRIER	= 0x10
} mbim_ctrl_caps_t;

/**
 * \struct mbim_device_caps_info_t
 * \brief User response after parsing when doing a request for the device capabilities.
 */
typedef struct {
	/** Device type */
	mbim_device_type_t device_type;
	/** Cellular class */
	mbim_cellular_class_t cellular_class;
	/** Voice class */
	mbim_voice_class_t voice_class;
	/** Sim class */
	mbim_sim_class_t sim_class;
	/** Data class */
	mbim_data_class_t data_class;
	/** SMS caps */
	mbim_sms_caps_t sms_caps;
	/** Control caps */
	mbim_ctrl_caps_t control_caps;
	/** Max session supported by the device */
	uint32_t max_sessions;
	/** Available when data class is MBIM_DATA_CLASS_CUSTOM */
	char *custom_data_class;
	/** GSM-based devices: IMEI, CDMA-based: ESN or MEID */
	char *device_id;
	/** Firmware specific info. Max size is 15 bytes in UTF-8. */
	char *firmware_info;
	/** Hardware specific info. Max size is 15 bytes in UTF-8. */
	char *hardware_info;
} mbim_device_caps_info_t;

/**
 * Query the device capabilities. The response will be mbim_device_caps_info_t..
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_device_caps(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/* MBIM CID REGISTER STATE as defined in MBIM spec v1.0 on p109 */

/**
 * \enum mbim_register_action_t
 * \brief The register action. Set the function to automatic/manual registration mode.
 */
typedef enum {
	MBIM_REGISTER_ACTION_AUTOMATIC,
	MBIM_REGISTER_ACTION_MANUAL
} mbim_register_action_t;

/**
 * \enum mbim_register_state_t
 * \brief The register state of the device
 */
typedef enum {
	MBIM_REGISTER_STATE_UNKNOWN,
	MBIM_REGISTER_STATE_DEREGISTERED,
	MBIM_REGISTER_STATE_SEARCHING,
	MBIM_REGISTER_STATE_HOME,
	MBIM_REGISTER_STATE_ROAMING,
	MBIM_REGISTER_STATE_PARTNER,
	MBIM_REGISTER_STATE_DENIED
} mbim_register_state_t;

/**
 *\enum mbim_register_mode_t
 *\brief The register mode of the device. This only affects network selection mode.
 */
typedef enum {
	MBIM_REGISTER_MODE_UNKNOWN,
	MBIM_REGISTER_MODE_AUTOMATIC,
	MBIM_REGISTER_MODE_MANUAL
} mbim_register_mode_t;

/**
 * \enum mbim_registration_flags_t
 * \brief Bitmap that indicates network conditions related to autoattach and manual network selection capabilities
 */
typedef enum {
	MBIM_REGISTRATION_NONE,
	MBIM_REGISTRATION_MANUAL_SELECTION_NOT_AVAILABLE,
	MBIM_REGISTRATION_PACKET_SERVICE_AUTOMATIC_ATTACH
} mbim_registration_flags_t;

/**
 * \struct mbim_set_registration_state_raw_t
 * \brief Raw struct to set the registration state of the function
 */
typedef struct {
	uint32_t provider_id_offset;
	uint32_t provider_id_size;
	mbim_register_action_t register_action;
	mbim_data_class_t data_class;
	uint8_t data_buffer[];
} mbim_set_registration_state_raw_t;

/**
 * \struct mbim_set_registration_state_t
 * \brief Struct to set the registration state of the function
 */
typedef struct {
	char *provider_id;
	mbim_register_action_t register_action;
	mbim_data_class_t data_class;
} mbim_set_registration_state_t;

/**
 * \struct mbim_registration_state_info
 * \brief Struct which is returned after parsing by both the set and query request of mbim_cid_register_state
 */
typedef struct {
	uint32_t nw_error;
	mbim_register_state_t register_state;
	mbim_register_mode_t register_mode;
	mbim_data_class_t available_data_classes;
	mbim_cellular_class_t current_cellular_class;
	char *provider_id;
	char *provider_name;
	char *roaming_text;
	mbim_registration_flags_t registration_flag;
} mbim_register_state_info_t;

/**
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_register_state(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] set_reg_state Pointer to mbim_set_registration_state_t
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_set_register_state(mbim_context_t *ctx, mbim_user_data_cb cb, mbim_set_registration_state_t *set_reg_state, void *arg);

/**
 * \param[in] ctx The mbim device context.
 * \param[in] data_classes A bitmask of mbim_data_class_t. Specifies the preferred network type. eg. LTE and UMTS.
 * \param[in] cb User data callback
 * \param[in] provider_id Provider id of the manual network
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_set_registration_mode_manual(mbim_context_t *ctx, mbim_data_class_t data_classes, mbim_user_data_cb cb, char *provider_id, void *arg);

/**
 * \param[in] ctx The mbim device context.
 * \param[in] data_classes A bitmask of mbim_data_class_t. Specifies the preferred network type. eg. LTE and UMTS.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_set_registration_mode_automatic(mbim_context_t *ctx, mbim_data_class_t data_classes, mbim_user_data_cb cb, void *arg);

/* MBIM CID SUBSCRIBER READY as defined in MBIM spec v1.0 on p85 */

/**
 * \enum mbim_subscriber_ready_state_t
 * \brief Sim states
 */
typedef enum {
	/** The SIM has not yet completed initialization */
	MBIM_SUBSCRIBER_READY_STATE_NOT_INITIALIZED,
	/** The SIM is initialized, all subscriber fields in mbim_subscriber_ready_info_t are filled in */
	MBIM_SUBSCRIBER_READY_STATE_INITIALIZED,
	/** The SIM is not inserted */
	MBIM_SUBSCRIBER_READY_STATE_SIM_NOT_INSERTED,
	/** The SIM is inserted, but the device is invalid. I.e. bad pin and puk. */
	MBIM_SUBSCRIBER_READY_STATE_BAD_SIM,
	/** General SIM failure */
	MBIM_SUBSCRIBER_READY_STATE_FAILURE,
	/** Subscription is not activated */
	MBIM_SUBSCRIBER_READY_STATE_NOT_ACTIVATED,
	/** The SIM is locked and requires PIN1 or PUK1 to unlock */
	MBIM_SUBSCRIBER_READY_STATE_DEVICE_LOCKED
} mbim_subscriber_ready_state_t;

/**
 * \enum mbim_unique_id_flags_t
 * \brief Ready info
 */
typedef enum {
	/** Device is in normal mode */
	MBIM_READY_INFO_FLAGS_NONE,
	/** The host will not display the subscriber id specified in the same CID */
	MBIM_READY_INFO_FLAGS_PROTECT_UNIQUE_ID
} mbim_unique_id_flags_t;

/**
 * \struct mbim_subscriber_ready_info_t
 * \brief Response when querying for MBIM_CID_SUBSCRIBER_READY_STATUS
 */
typedef struct {
	/** Subscriber ready state */
	mbim_subscriber_ready_state_t ready_state;
	/** Subscriber id */
	char *subscriber_id;
	/** SIM iccid */
	char *sim_iccid;
	/** Ready info */
	mbim_unique_id_flags_t ready_info;
	/** Amount of telephone numbers */
	uint32_t telephone_count;
	/** Pointer to an array of telephone numbers */
	char **telephone_numbers;
} mbim_subscriber_ready_info_t;

/**
 * Query the subscriber status (including the sim state).
 * The response will be an mbim_subscriber_ready_info_t struct.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_subscriber_ready_status(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);


/* MBIM CID SIGNAL STATE as defined in MBIM spec v1.0 on p126 */

/**
 * \struct mbim_signal_state_info_t
 * \brief info about the signal strength
 */
typedef struct {
	/** The rssi */
	uint32_t rssi;
	/** Error Rate */
	uint32_t error_rate;
	/** SignalStrengthInterval: Reporting interval, in seconds */
	uint32_t signal_strength_interval;
	/**RssiThreshold: The difference in RSSI coded values that trigger a report */
	uint32_t rssi_threshold;
	/** ErrorRateThreshold: The difference in ErrorRate coded values that trigger a report. 0xffffffff for don't care. */
	uint32_t error_rate_threshold;
} mbim_signal_state_info_t;

/**
 * \struct mbim_set_signal_state_t
 * \brief struct used to set the signal state options.
 */
typedef struct {
	/** The reporting interval in seconds */
	uint32_t signal_strength_interval;
	/** The difference in RSSI coded values that trigger a report. 0 = default function behaviour */
	uint32_t rssi_threshold;
	/** The difference in ErrorRate coded values that trigger a report. 0xffffffff for don't care */
	uint32_t error_rate_threshold;
} mbim_set_signal_state_t;

typedef enum {
	MBIM_SIGNAL_STATE_TIME_BASED_MODE,
	MBIM_SIGNAL_STATE_THRESHOLD_BASED_MODE
} mbim_signal_state_mode_t;

/**
 * Query the signal state.
 * The response will be an mbim_signal_state_info_t structure
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_signal_state(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * Set the signal state.
 * The response will be an mbim_signal_state_info_t structure
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] signal_state Pointer to mbim_set_signal_state_t struct.
 * \param[in] mode Used for notifications. i
 *		MBIM_SIGNAL_STATE_TIME_BASED_MODE or MBIM_SIGNAL_STATE_THRESHOLD_MODE.
 *		* Time based: rssi_threshold and error_rate_threshold are ignored.
 *		* Threshold based: signal_strength_interval is ignored.
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_set_signal_state(mbim_context_t *ctx, mbim_user_data_cb cb, mbim_set_signal_state_t *signal_state, mbim_signal_state_mode_t mode, void *arg);

/* MBIM CID PACKET SERVICE as defined in MBIM spec v1.0 on p123 */

/**
 * \enum mbim_packet_service_action_t
 * \brief Attach or detach to the registered providers's network.
 */
typedef enum {
	MBIM_PACKET_SERVICE_ACTION_ATTACH,
	MBIM_PACKET_SERVICE_ACTION_DETACH
} mbim_packet_service_action_t;

/**
 * \enum mbim_packet_service_state_t
 * \brief Packet service state to the registered network.
 */
typedef enum {
	MBIM_PACKET_SERVICE_STATE_UNKNOWN,
	MBIM_PACKET_SERVICE_STATE_ATTACHING,
	MBIM_PACKET_SERVICE_STATE_ATTACHED,
	MBIM_PACKET_SERVICE_STATE_DETACHING,
	MBIM_PACKET_SERVICE_STATE_DETACHED
} mbim_packet_service_state_t;

/**
 * \struct mbim_packet_service_info_t
 * \brief Response after a set/query/notification for the MBIM_CID_PACKET_SERVICE.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** Network error (3GPP TS 24.008 or p112 in the mbim spec v1.0) */
	uint32_t nw_error;
	/** Packet service state */
	mbim_packet_service_state_t packet_service_state;
	/** Available data class (LTE, HSPA, ...) */
	mbim_data_class_t highest_available_data_class;
	/** Uplink bit rate, in bits per second */
	uint64_t uplink_speed;
	/** Downlink bit rate, in bits per second */
	uint64_t downlink_speed;
} mbim_packet_service_info_t;

/**
 * \struct mbim_set_packet_service_t;
 * \brief Struct used in the information buffer when setting the MBIM_CID_PACKET_SERVICE.
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	mbim_packet_service_action_t packet_service_action;
} mbim_set_packet_service_t;


/**
 * Attach the registered network. We must be attached before sending a connect call.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_packet_service_attach(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * Detach from the registered network.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_packet_service_detach(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * Query the packet service info. The response is a mbim_packet_service_info_t struct.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_packet_service(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * \struct mbim_context_t
 * \brief An array of mbim_context_t is return after a set or query of mbim_cid_provisioned context.
 * It can be used to provision the APN, username and password over-the-air (OTA)
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** A unique ID for this context. On set, use 0xFFFFFFFF for auto indexing in the table. 0xFFFFFFFF 
	 * cannot be returned in a response */
	uint32_t context_id;
	/** Type of the context */
	mbim_context_type_t context_type;
	/** Access string (APN) for the context_type. Max size is 100 characters */
	char *access_string;
	/** Username for the context_type */
	char *username;
	/** Password for the context_type */
	char *password;
	/** Specifies the compression to be used in the data connection (Only for GSM-based devices) */
	mbim_compression_t compression;
	/** Authentication type to use for the PDP activation */
	mbim_auth_protocol_t auth_protocol;
} mbim_provisioned_context_t;

/**
 * \struct mbim_provisioned_context_t
 * \brief Contains an array of provisioned contexts
 */
typedef struct __attribute__ ((packed, aligned(4))) {
	/** Number of provisioned contexts */
	uint32_t count;
	/** Array of mbim_context_t structs */
	mbim_provisioned_context_t context[];
} mbim_provisioned_contexts_t;

/**
 * Query the provisioned context. The response is a mbim_provisioned_contexts_t struct.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_provisioned_contexts(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * \struct mbim_set_provisioned_context_t
 * \brief Struct used in a set command for cid MBIM_CID_PROVISIONED_CONTEXTS.
 */
typedef struct {
	/** A unique context ID for this context. Device handles context_id if set to UINT32_MAX */
	uint32_t context_id;
	/** Specifies the type of context being represented */
	mbim_context_type_t context_type;
	/** Offset from the beginning of this struct to the access string in the data_buffer */
	uint32_t access_string_offset;
	/** The size used for the access string */
	uint32_t access_string_size;
	/** Offset from the beginning of this struct to the username in the data_buffer */
	uint32_t user_name_offset;
	/** Size used for the username */
	uint32_t user_name_size;
	/** Offset fromt he beginning of this struct to the password in the data_buffer */
	uint32_t password_offset;
	/** Size used for the password */
	uint32_t password_size;
	/** Specifies the compression being used in the data connection */
	mbim_compression_t compression;
	/** Specifies the authentication type */
	mbim_auth_protocol_t auth_protocol;
	/** Offset from the beginning of this struct to the provider id in the data_buffer */
	uint32_t provider_id_offset;
	/** Size used for the provider id */
	uint32_t provider_id_size;
	/** The data buffer */
	uint8_t data_buffer[];
} mbim_set_provisioned_context_t;

/**
 * \struct mbim_set_context_t
 * \brief Struct used for setting the provisioned context.
 */
typedef struct {
	/** The context id. Set to UINT32_MAX if the mbim device needs to handle this. */
	uint32_t context_id;
	/** The context type */
	mbim_context_type_t context_type;
	/** The access string (APN). This cannot be NULL or empty. */
	char *access_string;
	/** The username. Can be empty. */
	char *username;
	/** The password. Can be empty. */
	char *password;
	/** The compression being used. */
	mbim_compression_t compression;
	/** The authentication protocol */
	mbim_auth_protocol_t auth_protocol;
	/** The provider id. Cannot be NULL or empty */
	char *provider_id;
} mbim_set_context_t;

/**
 * Setthe provisioned context. The response is a mbim_provisioned_contexts_t struct.
 * \param[in] ctx The mbim device context.
 * \param[in] provisioned_context The provisioned context struct which contains the access string, username, ...
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_set_provisioned_contexts(mbim_context_t *ctx, mbim_set_context_t *provisioned_context, mbim_user_data_cb cb, void *arg);

/**
 * \struct mbim_device_service_element_t
 * \brief Struct representing one device service with it's supported command IDs.
 */
typedef struct {
	/** The device service */
	mbim_services_t service;
	/** DSS payload support from host to device */
	uint32_t dss_host_to_device;
	/** DSS payload support from device to host */
	uint32_t dss_device_to_host;
	/** Max amount of instances of this service, which device can handle. */
	uint32_t max_dss_instances;
	/** Number of command IDs supported for this device service. Number of elements in cid array */
	uint32_t cid_count;
	/** The command IDs which are supported by this device service. */
	mbim_cid_t cid[];
} mbim_device_service_element_t;

/**
 * \struct mbim_device_services_info_t
 * \brief Struct returned after querying the device services.
 */
typedef struct {
	/** Number of mbim_device_service_element_t structs in the device service element (dse) array. */
	uint32_t device_services_count;
	/** The maximum number of activated DSS sessions that are supported by the function. */
	uint32_t max_dss_sessions;
	/** Pointer to array of device services */
	mbim_device_service_element_t *dse[];
} mbim_device_services_info_t;

/**
 * Query the supported MBIM commands on the module.
 * \param[in] ctx The mbim device context.
 * \param[in] cb User data callback
 * \param[in] arg Extra argument for the user callback
 * \return MBIM_OK, MBIM_ERROR, MBIM_INVALID_PARAMETER.
 */
uint32_t mbim_get_device_services(mbim_context_t *ctx, mbim_user_data_cb cb, void *arg);

/**
 * \enum mbim_nw_error_t
 * \brief 3GPP TS 24.008 cause codes for nw_error.
 */
typedef enum {
	MBIM_NW_ERROR_IMSI_UNKNOWN_HLR = 2,
	MBIM_NW_ERROR_IMSI_UNKNOWN_VLR = 4,
	MBIM_NW_ERROR_ILLEGAL_ME = 6,
	MBIM_NW_ERROR_GPRS_NOT_ALLOWED,
	MBIM_NW_ERROR_GPRS_NON_GRPS_NOT_ALLOWED,
	MBIM_NW_ERROR_PLMN_NOT_ALLOWED = 11,
	MBIM_NW_ERROR_LOCATION_AREA_NOT_ALLOWED,
	MBIM_NW_ERROR_ROAMING_NOT_ALLOWED,
	MBIM_NW_ERROR_GPRS_NOT_ALLOWED_IN_PLMN,
	MBIM_NW_ERROR_NO_SUITABLE_CELLS,
	MBIM_NW_ERROR_NETWORK_FAILURE = 17,
	MBIM_NW_ERROR_CONGESTION = 22
} mbim_nw_error_t;


/** @} */

#endif

