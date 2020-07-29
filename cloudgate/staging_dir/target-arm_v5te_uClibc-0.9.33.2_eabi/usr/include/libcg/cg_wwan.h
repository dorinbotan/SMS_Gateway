#ifndef __CG_WWAN_H__
#define __CG_WWAN_H__

#include <time.h>

#include <libcg/cgate.h>

CG_BEGIN_DECLS

/**
* \file cg_wwan.h
* \brief Header file containing all definitions/functions related specific to wwan (3G) networking.
*/

/**
* \defgroup wwan WWAN
* @{
*/

/** Maximum size of network names */
#define CG_NETWORK_NAME_SIZE 64
/** Maximum size of image names */
#define CG_MAX_IMAGE_NAME 64

/** Definition of the modem device names */
#define GOBI_DEVICE "usb0"
#define TELIT_DEVICE "wwan0"
#define HPEX_DEVICE "hso0"

/**
* \enum cg_wwan_pin_type_t Different PIN states
*/
typedef enum {
	/** Unknown PIN state */
	CG_WWAN_PIN_UNKNOWN = 0,
	/** SIM is not inserted */
	CG_WWAN_PIN_SIM_NOT_INSERTED,
	/** PIN is ready & no further action required */
	CG_WWAN_PIN_READY,
	/** PIN code needs to be entered */
	CG_WWAN_PIN_SIM_PIN,
	/** PUK code needs to be entered */
	CG_WWAN_PIN_SIM_PUK,
	/** An error occurred while trying to initialize the SIM */
	CG_WWAN_PIN_SIM_FAILURE,
	/** The SIM is locked to a different network, an unlock code is required */
	CG_WWAN_PIN_LOCKED_PN,
	/** A SIM was inserted which is not supported */
	CG_WWAN_PIN_SIM_NOT_SUPPORTED,
} cg_wwan_pin_type_t;

/**
* \enum cg_wwan_system_t
* \brief Enumeration describing the possible mobile sytem technologies offered
* by the serving cell.\n Cells can indicate combinations of technologies
* , eg CG_SYS_WCDMA & CG_SYS_HSDPA & CG_SYS_HSUPA in general all represent 3G.
*/
typedef enum {
	CG_SYS_NONE		= 0x1,
	CG_SYS_GSM		= 0x2,
	CG_SYS_GPRS		= 0x4,
	CG_SYS_EDGE		= 0x8,
	CG_SYS_WCDMA		= 0x10,
	CG_SYS_HSDPA		= 0x20,
	CG_SYS_HSUPA		= 0x40,
	CG_SYS_HSPA		= 0x80, //HSDPA and HSUPA
	CG_SYS_HSDPAPLUS	= 0x100, //HSDPA+
	CG_SYS_HSPAPLUS		= 0x200, //HSDPA+ and HSUPA
	// CG_SYS_LTE is mapped to 0x400 in order to be able to use a 16 bit wide sytem_type value and keep the SDK api unchanged
	CG_SYS_LTE          = 0x400,
	CG_SYS_CDMA		= 0x1000 ,
	CG_SYS_EVDO		= 0x2000, //EVDO, HDR type uknown
	CG_SYS_EVDOREL0		= 0x4000, //EVDO REL0
	CG_SYS_EVDOREVA		= 0x8000, //EVDO REV A

} cg_wwan_system_t;

/**
* \struct cg_wwan_network_t
* \brief Structure defining a network
*/
typedef struct {
	/** Name of the network */
	char name[CG_NETWORK_NAME_SIZE];
	/** Type of the system. Mask of \ref cg_wwan_system_t */
	uint16_t system_type;
	/** Network ID: MCC / MNC concatenation */
	uint32_t id;
	/** Byte to indicate if this is a preferred network. */
	uint8_t preferred;
	/** Byte to indicate that the device can roam on that network */
	uint8_t roaming;
	/** Byte to indicate that it is forbidden to roam on that network */
	uint8_t forbidden;
	/** byte to indicate that it is a home network */
	uint8_t home;
} cg_wwan_network_t;

/**
* \struct cg_wwan_network_list_t
* \brief Structure for a list of networks
*/
typedef struct {
	/** The number of entries in networks */
	uint32_t entries;
	/** The actual networks */
	cg_wwan_network_t networks[0];
} cg_wwan_network_list_t;

/**
* \enum cg_wwan_mode_t
* \brief Indicate in what mode the WWAN device is. In case of CG_WWAN_TYPE_CDMA, you can
* use the CDMA-specific calls, but not the UMTS-specific calls. (Other way around for CG_WWAN_TYPE_UMTS)
*/
typedef enum {
	/** CDMA mode */
	CG_WWAN_MODE_CDMA = 0,
	/** UMTS mode */
	CG_WWAN_MODE_UMTS,
} cg_wwan_mode_t;

/**
* \enum cg_wwan_creg_status_t
* \brief Indicate in what network registration status the WWAN device is.
*/
typedef enum {
	/** Not registered, WWAN device is not currently searching a new operator to register to. */
	CG_WWAN_CREG_NOT_REG = 0,
	/** Registered, home network */
	CG_WWAN_CREG_REG = 1,
	/** Not registered, but WWAN device is currently searching a new operator to register to. */
	CG_WWAN_CREG_SEARCHING = 2,
	/** Registration denied. */
	CG_WWAN_CREG_DENIED = 3,
	/** Registered, roaming. */
	CG_WWAN_CREG_ROAMING = 5,
} cg_wwan_creg_status_t;

/**
* \struct cg_wwan_reg_state_t
* \brief Network registration state
*/
typedef struct {
	/** Circuit-switched registration state */
	cg_wwan_creg_status_t cs_state;
	/** Packet-switched registration state */
	cg_wwan_creg_status_t ps_state;
	/** Type of system on which the device is registered. */
	cg_wwan_system_t system_type;
	/** The name of the network on which we're registered */
	char network_name[CG_NETWORK_NAME_SIZE];
} cg_wwan_reg_state_t;

/**
* \struct cg_wwan_pin_state_t
* \brief PIN code status of the SIM inserted in the device
*/
typedef struct {
	/** The current PIN state */
	cg_wwan_pin_type_t pin_type;
	/** The number of PIN retries remaining on the SIM */
	uint32_t pin_retries;
	/** The number of PUK retries remaining on the SIM */
	uint32_t puk_retries;
	/** Set to 1 if PIN code is enabled, 0 is PIN code is disabled */
	uint8_t pin_enabled;
} cg_wwan_pin_state_t;

/**
* \enum cg_wwan_act_state_t The state of CDMA activation
*/
typedef enum {
	/** Service is not activated */
	CG_WWAN_ACT_STATUS_NOT_ACTIVATED =   0x00,
	/** Service is activated */
	CG_WWAN_ACT_STATUS_ACTIVATED =       0x01,
	/** Activation connecting – Network connection is in progress for automatic activation of service */
	CG_WWAN_ACT_STATUS_CONNECTION =      0x02,
	/** Activation connected – Network connection is connected for automatic activation of service */
	CG_WWAN_ACT_STATUS_CONNECTED =       0x03,
	/** OTASP security is authenticated */
	CG_WWAN_ACT_STATUS_OTASP_AUTH =      0x04,
	/** OTASP NAM is downloaded */
	CG_WWAN_ACT_STATUS_OTASP_NAM =       0x05,
	/** OTASP MDN is downloaded */
	CG_WWAN_ACT_STATUS_OTASP_MDN =       0x06,
	/** OTASP IMSI downloaded */
	CG_WWAN_ACT_STATUS_OTASP_IMSI =      0x07,
	/** OTASP PRL is downloaded */
	CG_WWAN_ACT_STATUS_OTASP_PRL =       0x08,
	/** OTASP SPC is downloaded */
	CG_WWAN_ACT_STATUS_OTASP_SPC =       0x09,
	/** OTASP settings are committed */
	CG_WWAN_ACT_STATUS_OTASP_COMMITTED = 0x0A,
} cg_wwan_act_state_t;

/**
* \struct cg_wwan_img_t
* \brief Image name
*/
typedef struct {
	/** Name of the image */
	char name[CG_MAX_IMAGE_NAME];
} cg_wwan_img_t;

/**
* \struct cg_wwan_img_list_t
* \brief Structure containing a list of available images
*/
typedef struct {
	/** Number of entries */
	uint32_t entries;
	/** Current active entry */
	uint32_t current;
	/** Array of images names */
	cg_wwan_img_t images[];
} cg_wwan_img_list_t;

/**
* \struct cg_wwan_diag_param_t
* \brief Diagnostic parameter information
*/
typedef struct {
	/** Parameter name */
	const char *name;
	/** Parameter value */
	const char *value;
} cg_wwan_diag_param_t;


/**
* \enum cg_wwan_sim_switch_mode_t The sim mode used for sims in the Telematics expansion card
*/
typedef enum {
	/** The sim slot with the best functionality is chosen automatically */
	CG_SIM_SWITCH_MODE_AUTO,
	/** The sim in slot 1 will be used as active sim
	 *  This is the sim closest to the PCB */
	CG_SIM_SWITCH_MODE_SIM1,
	/** The sim in slot 2 will be used as active sim
	 *  This is the sim farthest from the PCB */
	CG_SIM_SWITCH_MODE_SIM2
} cg_wwan_sim_switch_mode_t;

/**
* \enum cg_wwan_sim_switch_status_t The sim status shows the active sim on the Telematics expansion card
*/
typedef enum {
	/** The sim in slot 1 is currently the active sim
	 *  This is the sim closest to the PCB */
	CG_SIM_SWITCH_STATE_SIM1 = 1,
	/** The sim in slot 2 is currently the active sim
	 *  This is the sim farthest from the PCB */
	CG_SIM_SWITCH_STATE_SIM2 = 2
} cg_wwan_sim_switch_state_t;

/**
* \enum cg_wwan_auth_type_t
* \brief The authentication type for the mobile connection.
*/
typedef enum {
	/** No authentication protocol is used */
	CG_WWAN_AUTH_TYPE_NONE,
	/** Password Authentication Protocol is used */
	CG_WWAN_AUTH_TYPE_PAP,
	/** Challenge Handshake Authentication Protocol is used */
	CG_WWAN_AUTH_TYPE_CHAP,
	/** Automatic authentication is used */
	CG_WWAN_AUTH_TYPE_AUTO,
} cg_wwan_auth_type_t;

/**
* \struct cg_wwan_network_settings_t
* \brief Struct used to set/get the mobile network settings
*/
typedef struct {
	/** Access Point Name */
	char apn[100 + 1];
	/** The username */
	char username[50 + 1];
	/** The password */
	char password[50 + 1];
	/** The authentication type */
	cg_wwan_auth_type_t auth_type;
} cg_wwan_network_settings_t;

/**
 * \struct cg_wwan_location_info_t
 * \brief Struct defining the location of the base transceiver station
 */

typedef struct {
	/** Location Area Code is a unique number to identify location area of a base station ( for UMTS and GSM networks) (equals zero if not supplied). */
	uint32_t lac;
	/** Tracking Area Code identifies a tracking area within a particular network ( for LTE networks) (equals zero if not supplied). */
	uint32_t tac;
	/** A unique number to identify base transceiver station (BTS) or sector of a BTS ( equals zero if not supplied). */
	uint32_t cell_id;
} cg_wwan_location_info_t;

/** Callback when requesting a network scan
* \param[in] status Status of network scan. CG_STATUS_OK or CG_STATUS_ERROR.
* \param[in] list The network list. This list is allocated by the caller of the callback
* and will be freed when the callback returns.
* \param[in] context The context which was passed on in cg_wwan_search_networks
*/
typedef void (*cg_wwan_network_list_cb_t)(cg_status_t status, cg_wwan_network_list_t *list, void *context);

/** Callback for registration state updates
* \param[in] dev_name The device for which the registration state is updated
* \param[in] state The updated state. Structure allocated by caller and will be free when the
* callback returns.
* \param[in] context The context which was passed on in cg_wwan_register_reg_state_notification
*/
typedef void (*cg_wwan_reg_state_cb_t)(const char *dev_name, cg_wwan_reg_state_t *state, void *context);

/**\name Generic functions */
/**@{*/

/** Get the name of the primary active device
* \param[out] dev_name will be filled in by the callee in case of success. Caller will need to free the string.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case an invalid parameter is provided
* or CG_STATUS_ERROR in case no primary WWAN device present.
*/
cg_status_t cg_wwan_get_active_primary_dev(char **dev_name);

/**@{*/

/** Get the mode which the WWAN device is currently in
* \param[in] dev_name WWAN device name.
* \param[out] mode Device mode
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_mode(const char *dev_name, cg_wwan_mode_t *mode);

/** Get the current signal strength
* \param[in] dev_name WWAN device name
* \param[out] strength Signal strength represented in dBm. Always negative, 0 when no signal present.\n
* The exact meaning depends on the WWAN technology of the selected cell:\n
* GSM/GPRS/EDGE (2G): RSSI (Received Signal Strength Indication)\n
* WCDMA/HSDPA/HSUPA/HSPA/HSDPA+/HSPA+ (3G): RSCP (Received Signal Code Power)\n
* LTE (4G): RSRP (Reference Signal Received Power)\n
* CDMA/EVDO: Signal strength of forward link pilot
* \param[out] ecio ECIO represented in dB. Always negative, 0 when no signal present.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_signal_strength(const char *dev_name, int8_t *strength, int8_t *ecio);

/** Enable or disable the WWAN radio. The transition to disabled can take a little time as the
*   modem is detaching from the network. cg_wwan_get_radio can be polled to double check when
*   the transition has completed.
* \param[in] dev_name WWAN device name
* \param[in] enabled Set to TRUE to enable radio. FALSE to disable.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_set_radio(const char *dev_name, int enabled);

/** Get the current WWAN radio state.
* \param[in] dev_name WWAN device name
* \param[out] enabled Returns TRUE if radio is enabled. FALSE if disabled.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_radio(const char *dev_name, int *enabled);

/** Get the current registration state
* \param[in] dev_name WWAN device name
* \param[out] state The registration state
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_reg_state(const char *dev_name, cg_wwan_reg_state_t *state);

/** Register for registration state notification for a WWAN device.
* \param[in] dev_name WWAN device name
* \param[in] cb Callback which is called when the registration state changes
* \param[in] context Context parameter passed on to the callback when called.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_register_reg_state_notification(const char *dev_name, cg_wwan_reg_state_cb_t cb, void *context);

/** Deregister for registration state notifications
* \param[in] dev_name WWAN device name
* \param[in] cb Callback to deregister
* \param[in] context Context parameter,97
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_deregister_reg_state_notification(const char *dev_name, cg_wwan_reg_state_cb_t cb, void *context);

/** Get the serial number of the WWAN device.
* \param[in] dev_name WWAN device name
* \param[out] serial The device serial. *serial will be filled in by the callee. Caller will need to free the string.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_serial(const char *dev_name, char **serial);

/** Get a list containing diagnostic information \n Only supported by the GOBI_DEVICE
* \param[in] dev_name WWAN device name
* \param[out] num_params When successful, will be set to the number of diagnostic parameters in the array
* \param[out] params When successful, will be set to an array of cg_wwan_diag_param_t, allocated by the callee.
* The caller will be responsible for freeing the array only, not memory pointed to by members of cg_wwan_diag_param_t.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_diagnostics(const char *dev_name, uint32_t *num_params, cg_wwan_diag_param_t **params);

/** Sets the mobile network settings.
* \param[in] dev_name WWAN device name
* \param[in] wwan_network_settings Pointer to struct of type cg_wwan_network_settings_t including the APN, username, password, authentication method.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
* \note The sim must be present, otherwise you get a CG_STATUS_ERROR.
*/
cg_status_t cg_wwan_set_network_settings(const char *dev_name, cg_wwan_network_settings_t *wwan_network_settings);

/** Gets the mobile network settings.
* \param[in] dev_name WWAN device name
* \param[out] wwan_network_settings Callee will set *wwan_network_settings to a cg_wwan_network_settings_t pointer. Caller needs
* to free this pointer.
* \note: The password is always an empty string. It is not readable.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
* \note The sim must be present, otherwise you get a CG_STATUS_ERROR.
*/
cg_status_t cg_wwan_get_network_settings(const char *dev_name, cg_wwan_network_settings_t **wwan_network_settings);

/** Get the phone number from the SIM or MDN
 * \param[in] dev_name The device name.
 * \param[out] phone_number The phone number from the SIM or MDN. If no phone number is present, "unknown" is returned. Caller needs to free this pointer.
 * \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
 * or CG_STATUS_ERROR in case of an error.
 */
cg_status_t cg_wwan_get_phone_number(const char *dev_name, char **phone_number);


/**@}*/

/**\name  UMTS - specific functions */
/**@{*/

/** Search for UMTS networks
* \param[in] dev_name WWAN device name
* \param[in] cb Callback called when network scan is completed.
* \param[in] context This parameter will be passed on to the callback
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter,
* CG_STATUS_ERROR in case of an error or CG_STATUS_RESOURCE_UNAVAILABLE when the WWAN device is
* connected to a network.
*/
cg_status_t cg_wwan_search_networks(const char *dev_name, cg_wwan_network_list_cb_t cb, void *context);

/** Get the IMEI of the WWAN device
* \param[in] dev_name WWAN device name
* \param[out] imei The device IMEI. *imei will be filled in the callee. Called will need to free.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_imei(const char *dev_name, char **imei);

/** Get the IMSI of the SIM inserted in the device
* \param[in] dev_name WWAN device name
* \param[out] imsi IMSI of the device. *imsi will be filled in the callee. Called will need to free.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_imsi(const char *dev_name, char **imsi);

/** Get the ICCID of the SIM inserted in the device
* \param[in] dev_name WWAN device name
* \param[out] iccid ICCID of the device. *iccid will be filled in the callee. Called will need to free.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_iccid(const char *dev_name, char **iccid);

/**
* \param[in] dev_name WWAN device name
* \param[out] pin_state PIN state of the device.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_pin_get_state(const char *dev_name, cg_wwan_pin_state_t *pin_state);

/** Save the pin (on next submission), or disable (and delete the saved pin)
* \param[in] dev_name WWAN device name
* \param[in] enabled Set to TRUE if you want the device to save the PIN so it doesn't need to be entered
* every time. Set to FALSE if you want the device to always ask for the PIN code.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_save_pin(const char *dev_name, uint8_t enabled);

/**
* \param[in] dev_name WWAN device name
* \param[in] pin Pin code
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_submit_pin(const char *dev_name, const char *pin);

/**
* \param[in] dev_name WWAN device name
* \param[in] old_pin_puk The old PIN code if you want to change your current PIN. If the PIN is blocked
* because a wrong PIN code was entered too many times, this field should contain a PUK code instead.
* \param[in] new_pin The new PIN code
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_change_pin(const char *dev_name, const char *old_pin_puk, const char *new_pin);

/**
* \param[in] dev_name WWAN device name
* \param[in] enabled Set to TRUE to enable the PIN code. FALSE to disable.
* \param[in] pin The current PIN code.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_pin_set_enabled(const char *dev_name, uint8_t enabled, const char *pin);

/** Get the WWAN UMTS network selection mode
* \param[in] dev_name WWAN device name
* \param[out] automatic Is set to 1 in case of automatic network selection, set to 0 in case of manual network selection.
* \param[out] network The network on which the device is registered or tries to register.
* Note: The network settings preferred, roaming, forbidden and home are set to 0xFF if they are unknown.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_nw_selection_mode(const char *dev_name, uint8_t *automatic, cg_wwan_network_t *network);

/** Set the WWAN UMTS network selection mode
* \param[in] dev_name WWAN device name
* \param[in] automatic Set to TRUE to set network selection mode to Automatic. In this case the device will search for
* a proper network himself and the network parameter will be ignored. If auto is set to FALSE, a network will need to be specified.
* \param[in] network The network on which to manually register. This is usually an entry which was returned in the network scan
* (\ref cg_wwan_search_networks).
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_set_nw_selection_mode(const char *dev_name, uint8_t automatic, cg_wwan_network_t *network);

/**@}*/

/** \name CDMA - specific functions */
/**@{*/

/**
* \note: TODO Add Sprint functions
*/

/** Get the CDMA MEID
* \param[in] dev_name WWAN device name
* \param[out] meid The MEID. *meid will be filled in the callee. Called will need to free.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_meid(const char *dev_name, char **meid);

/** Get the current state of the CDMA activation
* \param[in] dev_name WWAN device name
* \param[out] state The current state of the activation
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_activation_state(const char *dev_name, cg_wwan_act_state_t *state);

/** Initiate CDMA service activation
* \param[in] dev_name WWAN device name
* \param[in] act_string The activation string. Check your CDMA operator to see what it should be.
* Set to NULL in case of automatic activation.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_activate(const char *dev_name, const char *act_string);

/** Get the PRL version
* \param[in] dev_name WWAN device name
* \param[out] prl_version The PRL version.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_prl_version(const char *dev_name, uint16_t *prl_version);

/** Upload new PRL file. This file should be freely accessible and has to be located
* at /tmp/temp.prl. The user can remove this file afterwards.
* \param[in] dev_name WWAN device name
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_upload_prl(const char *dev_name);

/** Set whether the PRL file can be updated by the network. This only works for Sprint.
* \param[in] dev_name WWAN device name
* \param[in] enabled Set to TRUE to enable the network to push PRL updates. Set to FALSE to disable this
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_set_prl_network_update(const char *dev_name, uint8_t enabled);

/** Check whether the PRL file can be updated by the network. This only works for Sprint.
* \param[in] dev_name WWAN device name
* \param[out] enabled If enabled, the PRL file can be updated by the network.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_prl_network_update(const char *dev_name, uint8_t *enabled);

/**@}*/

/** \name Device - specific functions. */
/**@{*/

/** Get the name of the currently running image on the Gobi WWAN device
* \param[in] dev_name WWAN device name
* \param[out] image_name Name of the image
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_gobi_get_active_image(const char *dev_name, char **image_name);

/** Get the list of images on the Gobi WWAN device
* \param[in] dev_name WWAN device name
* \param[out] image_list List of images. *image_list will be allocated and set by callee.
* Caller will need to free
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_gobi_get_image_list(const char *dev_name, cg_wwan_img_list_t **image_list);

/** Set an image to be the active image on the Gobi device. The list of images can be
* queried through \ref cg_wwan_gobi_get_image_list. WARNING: The modem will be reset after
* calling this function. It can take more than 30 seconds for the modem to be completely initialized
* after this.
* \param[in] dev_name WWAN device name
* \param[in] image_name The name of the image to set active.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_gobi_set_active_image(const char *dev_name, const char *image_name);

/** Gets the type of a  Gobi image given the image name
* \param[in] dev_name WWAN device name
* \param[in] image_name Name of the image
* \param[out] image_type Callee will set *image_type to a string pointer. Caller needs to free this pointer.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_gobi_get_image_type(const char *dev_name, const char *image_name, char **image_type);


/** Reboots the current modem.
* Tip: The function cg_device_register_notification can be used to receive an update when the modem is present again.
* For this the device_mask CG_DEVTYPE_NETWORK should be used. In the callback we should look for the event CG_DEV_EVT_ARRIVED
* of the device with name "usb0", "hso0" or "wwan0", depending on the type of modem.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter,
* CG_STATUS_BUSY if the modem is busy or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_modem_reboot();

/** Sets the sim switch mode of the sim slots on the Telematics expansion card. To apply this sim mode, a modem reboot is necessary. (cg_wwan_modem_reboot).
* You need hardware version 1.3 or newer.
* \param[in] mode The sim switch mode to set.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_set_sim_switch(cg_wwan_sim_switch_mode_t mode);

/** Gets the sim switch status of the sim slots on the Telematics expansion card.
* You need hardware version 1.3 or newer.
* \param[out] mode Gets the sim switch mode.
* \param[out] current Gets the current sim switch state (which sim is currently active).
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_get_sim_switch(cg_wwan_sim_switch_mode_t *mode, cg_wwan_sim_switch_state_t *current);

/** Sets connection hunting for the given device.
* \param[in] dev_name The device name.
* \param[in] enabled Defines whether to enable/disable connection hunting.
* \param[in] fallback_time Defines the fallback time of the connection hunting.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wwan_set_connection_hunting(const char *dev_name, int enabled, int fallback_time);

/** Gets the location information of the base transceiver station.
 * \param[in] dev_name WWAN device name.
 * \param[out] location_info The location information, some attributes can be zero if not supported
 * \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter 
 * or CG_STATUS_ERROR in case of an error or function not supported or modem not registered.
 */
cg_status_t cg_wwan_get_location_info(const char *dev_name, cg_wwan_location_info_t **location_info);

/** @} */

/** @} */

CG_END_DECLS

#endif /* __CG_WWAN_H__ */
