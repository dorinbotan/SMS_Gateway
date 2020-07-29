#ifndef __CG_WLAN_H__
#define __CG_WLAN_H__

#include <libcg/cgate.h>

CG_BEGIN_DECLS

/**
* \file cg_wlan.h
* \brief Header file containing all definitions/functions related specific to WLAN networking. 
*/

/**
* \defgroup wlan WLAN
* @{
*/

/** Maximum length of the SSID */
#define CG_SSID_MAX_LENGTH 32
/** Maximum length of the password */
#define CG_PASSWORD_MAX_LENGTH 64

/**
* \enum cg_wlan_auth_t
* \brief Different WLAN authentication type
*/
typedef enum {
	/** Open authentication (= no authentication) */
	CG_WLAN_AUTH_OPEN = 0,
	/** WEP PSK authentication */
	CG_WLAN_AUTH_WEP,
	/** WPA-PSK authentication */
	CG_WLAN_AUTH_WPA_PSK,
	/** WPA2-PSK authentication */
	CG_WLAN_AUTH_WPA2_PSK,
	/** WPA-PSK or WPA2-PSK authentication */
	CG_WLAN_AUTH_WPA_WPA2_PSK,
	/** WPA-Enterprise authentication */
	CG_WLAN_AUTH_WPA_ENT,
	/** WPA2-Enterprise authentication */
	CG_WLAN_AUTH_WPA2_ENT,
	/** WPA-Enterprise or WPA2-Enterprise authentication */
	CG_WLAN_AUTH_WPA_WPA2_ENT,
} cg_wlan_auth_t;

/**
* \struct cg_wlan_auth_params_t
* \brief Authentication parameters for a WLAN client/access point
*/
typedef struct {
	/** Type of authentication */
	cg_wlan_auth_t type;
	/** Password used for PSK authentication */
	char password[CG_PASSWORD_MAX_LENGTH + 1];
} cg_wlan_auth_params_t;

/**
* \struct cg_wlan_network_t
* \brief Description of a WLAN network
*/
typedef struct {
	/** The SSID of the WLAN network */
	char ssid[CG_SSID_MAX_LENGTH + 1];
	/** Authentication type of the WLAN network */
	cg_wlan_auth_t auth_type;
	/** Configured channel of the WLAN network. If set to zero, automatic channel selection is enabled */
	uint16_t channel;
	/** Signal strength of the WLAN network */
	uint16_t signal_strength;
} cg_wlan_network_t;

/** Callback for \ref cg_wlan_sta_scan_networks. Will be called with a list of scanned networks.
* \ref cg_wlan_network_t will not contain any valid channel numbers.
* \param[in] status Status of the operation. CG_STATUS_SUCCESS or CG_STATUS_ERROR.
* \param[in] dev_name WLAN STA device name.
* \param[in] num_entries Number of entries in *networks*.
* \param[in] networks Array of cg_wlan_network_t structures.
* \param[in] context Context parameter passed on to \ref cg_wlan_sta_scan_networks.
*/
typedef void (*cg_wlan_network_list_cb_t)(cg_status_t status, const char *dev_name, uint32_t num_entries,
                                cg_wlan_network_t *networks, void *context);

/** Initiate a scan for WLAN networks.
* \param[in] dev_name WLAN STA device name.
* \param[in] cb Callback function that is called when the network scan has finished.
* \param[in] context User-defined context that is passed to the callback function.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_sta_scan_networks(const char *dev_name, cg_wlan_network_list_cb_t cb, void *context);

/** Add a network to the list of saved access points. An attempt will be made to make a network connection.
* The network with the best signal quality will be selected. The list of available networks
* can be queried with \ref cg_wlan_sta_scan_networks. This function can also be used to change
* the password for the specified WLAN network.
* \param[in] dev_name WLAN STA device name.
* \param[in] ssid The SSID of the network.
* \param[in] auth The authentication parameters. If it is not known which authentication methods
* are used by the access point, you can query the list of available networks with \ref cg_wlan_sta_scan_networks
* The result of that scan will contain the type of authentication which is necessary.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_sta_set_network(const char *dev_name, const char *ssid, cg_wlan_auth_params_t *auth);

/** Get information regarding the currently connected WLAN network.
* If we aren't connected to a WLAN access point, the SSID will not be filled in.
* \param[in] dev_name WLAN STA device name.
* \param[out] network Caller-allocated struct that will contain the relevant information on return.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_sta_get_connected_network(const char *dev_name, cg_wlan_network_t *network);

/** Get the list of saved access points.
* \ref cg_wlan_network_t entries in the list will not contain valid channel numbers and signal quality values.
* \param[in] dev_name WLAN STA device name.
* \param[out] num_entries When successful, the number of entries in *networks*.
* \param[out] networks When successful, will be set to an array of cg_wlan_network_t, allocated by the callee.
* The caller will be responsible for freeing the array.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_sta_get_network_list(const char *dev_name, uint32_t *num_entries, cg_wlan_network_t **networks);

/** Remove a network from the list of saved access points.
* \param[in] dev_name WLAN STA device name.
* \param[in] ssid SSID of the network.
* \param[in] auth Authentication type of the network.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_sta_remove_network(const char *dev_name, const char *ssid, cg_wlan_auth_t auth);

/** Set the SSID for the WLAN access point.
* \param[in] dev_name WLAN AP device name.
* \param[in] ssid The SSID to set for the access point.
* \param[in] broadcast A flag to indicate if the SSID should be broadcast or not. Set to FALSE to disable
* broadcast. In this case the network will be hidden & not visible for clients. In order to connect
* a client would need to know the SSID upfront. Set to TRUE to enable broadcast.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_ap_set_ssid(const char *dev_name, const char *ssid, uint8_t broadcast);

/** Get the SSID for the WLAN access point.
* \param[in] dev_name WLAN AP device name.
* \param[out] ssid SSID for the access point. Callee will set *ssid to the SSID string. Caller will
* need to free.
* \param[out] broadcast A flag to indicate if the SSID is broadcasted or not.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_ap_get_ssid(const char *dev_name, char **ssid, uint8_t *broadcast);

/** Set the WLAN channel for a WLAN access point interface.
* Changing the channel of a WLAN access point interface will change
* the channel for all access point interfaces on the same WLAN radio.
* If the WLAN station is enabled, changing the channel will not be possible since the access points
* have to follow the station's channel.
* \param[in] dev_name WLAN AP device name.
* \param[in] channel The WLAN channel. If set to zero, automatic channel selection is enabled.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter, CG_STATUS_ERROR in case of an error or
* CG_STATUS_RESOURCE_BUSY when the WLAN station is enabled.
*/
cg_status_t cg_wlan_ap_set_channel(const char *dev_name, uint16_t channel);

/** Get the WLAN channel for a WLAN access point interface.
* \param[in] dev_name WLAN AP device name.
* \param[in] channel WLAN channel to get. If channel is zero, automatic channel selection is enabled.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_ap_get_channel(const char *dev_name, uint16_t *channel);

/** Set the authentication parameters for a WLAN access point interface.
* CG_WLAN_AUTH_WEP isn't a valid authentication type for access point.
* \param[in] dev_name WLAN AP device name.
* \param[in] auth Authentication parameters.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_ap_set_auth_params(const char *dev_name, cg_wlan_auth_params_t *auth);

/** Get the authentication params for a WLAN access point interface.
* \param[in] dev_name WLAN AP device name.
* \param[out] auth Current authentication parameters of a WLAN access point interface.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_ap_get_auth_params(const char *dev_name, cg_wlan_auth_params_t *auth);

/** @} */

CG_END_DECLS

#endif /* __CG_WLAN_H__ */
