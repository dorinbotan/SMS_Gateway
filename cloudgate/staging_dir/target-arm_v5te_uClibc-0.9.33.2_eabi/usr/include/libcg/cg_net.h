#ifndef __CG_NET_H__
#define __CG_NET_H__

#include <libcg/cgate.h>

#include <sys/socket.h>
#include <netinet/in.h>

/** The maximum number of addresses which can be used for the connection watchdog */
#define CG_MAX_PING_ADDRESSES 5
/** The maximum length of the addresses in the connection watchdog */
#define CG_MAX_ADDRESS_LENGTH 64

#define CG_DEV_NAME_MAX 32

CG_BEGIN_DECLS

/**
* \file cg_net.h
* \brief Header file containing all definitions related to working with network interfaces, connecting, disconnecting...
*/

/**
* \note TODO:
*		+ Add tracking of number of clients connected to LAN interfaces
*/

/**
* \defgroup net Networking
* @{
*/

/**
* \enum cg_net_if_status_t
* \brief Interface status bitmasks
*/
typedef enum {
	/** Interface is enabled */
	CG_NET_INTERFACE_ENABLED = 0x1,
	/** Interface has proper IP address configuration */
	CG_NET_INTERFACE_CONFIGURED = 0x2,
	/** Interface is connected */
	CG_NET_INTERFACE_CONNECTED = 0x4,
	/** Possible to make a connection on interface */
	CG_NET_INTERFACE_CAN_CONNECT = 0x8,
} cg_net_if_status_t;

/**
* \enum cg_net_internet_status_t
* \brief The internet connection status. Not interface-specific but shows the general state of
* internet (WAN) connectivity of the device. For clients who just want to know if it is possible
* to reach the internet, without having to look at all the WAN interfaces to know if one is connected or not.
*/
typedef enum {
	/** The device does not have an internet connection */
	CG_NET_INTERNET_DISCONNECTED = 0,
	/** The device is trying to set up an internet connection on an interface */
	CG_NET_INTERNET_CONNECTING = 2,
	/** The device is connected to the internet */
	CG_NET_INTERNET_CONNECTED,
	/** The device has found a WAN interface to connect to, but the interface is configured to connect in on-demand mode,
	* which means that the interface will only be connected when there is traffic outgoing
	*/
	CG_NET_INTERNET_ON_DEMAND
} cg_net_internet_status_t;

/**
* \enum cg_net_zone_t
* \brief Enumeration to describe the different zones (LAN, WAN)
*/
typedef enum {
	/** LAN interface (pointed to internal network)*/
	CG_NET_ZONE_LAN = 0,
	/** WAN interface (pointed to internet) */
	CG_NET_ZONE_WAN,
} cg_net_zone_t;

/**
* \enum cg_net_mode_t
* \brief Describes the different connection modes
*/
typedef enum {
	/** The interface is always active */
	CG_NET_MODE_ALWAYS_ON = 0,
	/** The interface is only brought up when there is traffic wanting to get out on the interface. 
	* This is particularly useful for interfaces where charges are incurred based on connection duration
	* (phone lines, some 3G subscriptions)
	*/
	CG_NET_MODE_ON_DEMAND
} cg_net_mode_t;

/**
* \enum cg_net_if_type_t
* \brief Enumeration listing all the different types of possible interfaces
*/
typedef enum {
	/** Mobile 3G data interface */
	CG_NET_TYPE_WWAN = 0,
	/** Regular ethernet interface */
	CG_NET_TYPE_NIC,
	/** Wireless LAN Access Point interface */
	CG_NET_TYPE_WLAN_AP,
	/** Wireless LAN Station interface */
	CG_NET_TYPE_WLAN,
	/** Bridge interface */
	CG_NET_TYPE_BRIDGE,
	/** IPsec VPN interface */
	CG_NET_TYPE_IPSEC,
} cg_net_if_type_t;

/**
* \enum cg_net_ip_config_type_t
* \brief Type of IP address configuration
*/
typedef enum {
	/** Static IP address configuration */
	CG_STATIC = 0,
	/** Dynamic DHCP IP address configuration */ 
	CG_DHCP
} cg_net_ip_config_type_t;

/** 
* \enum cg_net_conn_strat_t
* \brief Enumeration of the different connection strategies. The device is able to execute different strategies
* to ensure connectivity automatically
*/
typedef enum {
	/** Manual strategy. All automatic behaviour is disabled. */
	CG_NET_STRATEGY_MANUAL = 0,
	/** Priority-based strategy. The device will try to maintain internet connectivity according to a priority list of interfaces.
	* If a certain interface connection fails, it will move to the next interface in the priority list. If a higher-ranked interface
	* becomes available again, it will try to move back to the higher-ranked interface.
	*/
	CG_NET_STRATEGY_PRIORITY,
} cg_net_conn_strat_t;


/**
* \struct cg_ip_addr_t
* \brief Structure describing a IP address
*/
typedef struct {
        uint8_t ipv6;
        union {
                in_addr_t ipv4;
                struct in6_addr ipv6;
        } ip;
} cg_ip_addr_t;

/**
* \struct cg_net_ip_config_t
* \brief Structure describing IP configuration of an interface
*/
typedef struct {
	/** IPv4 interface address */
	cg_ip_addr_t ip_addr;
	/** IPv4 netmask */
	cg_ip_addr_t netmask;
	/** MTU of the interface */
	int mtu;
} cg_net_ip_config_t;

/**
* \enum cg_net_lease_unit_t
* \brief Enumeration listing all the different units of the leasetime
*/
typedef enum
{
	/** leasetime unit minute  */
	CG_NET_UNIT_MINUTE = 0,
	/** leasetime unit hour  */
	CG_NET_UNIT_HOUR,
	/** leasetime unit day  */
	CG_NET_UNIT_DAY,
} cg_net_lease_unit_t;

/**
* \struct cg_net_leasetime_t
* \brief Structure describing a leasetime
*/
typedef struct
{
	/** Fill this in to override the leasetime amount which is passed on to DHCP clients */
	unsigned int amount;
	/** Fill this in to override the leasetime unit which is passed on to DHCP clients */
	cg_net_lease_unit_t unit;
} cg_net_leasetime_t;

/**
* \struct cg_net_dhcp_config_t
* \brief Structure used to configure DHCP server functionality
*/
typedef struct {
	/** Set to TRUE to enable DHCP server. FALSE to disable */
	int enabled;
	/** Starting address for the DHCP IP range */
	cg_ip_addr_t ip_start;
	/** End address for the DHCP IP range */
	cg_ip_addr_t ip_end;
	/** Fill this in to override the first DNS address which is passed on to DHCP clients */
	cg_ip_addr_t dns1;
	/** Fill this in to override the second DNS address which is passed on to DHCP clients */
	cg_ip_addr_t dns2;
	/** Fill this in to override the leasetime to DHCP clients */
	cg_net_leasetime_t leasetime;
} cg_net_dhcp_config_t;

/**
* \struct cg_net_lan_cfg_t
* \brief LAN-specific configuration settings for a network interface
*/
typedef struct {
	/** DHCP server configuration */
	cg_net_dhcp_config_t dhcp_config;

	/** Reference to bridge interface. If this parameter is non-NULL, it means the interface is part of a bridge.
	* This voids all previous IP config settings because the bridge IP configuration will determine the interface
	* configuration.
	*/
	/** Name of the bridge network interface */
	char bridge_dev_name[CG_DEV_NAME_MAX];
} cg_net_lan_cfg_t;

/**
* \struct cg_net_wan_cfg_t
* \brief WAN-specific configuration settings for a network interface
*/
typedef struct {
	/** Connection mode (always-on, on-demand,...)
	* In order for 'on-demand' to work, 'automatic' connecting must be enabled by calling cg_net_connect() first
	*/
	cg_net_mode_t mode;
	/** If the interface is set to on-demand mode (see \ref mode), this value indicates the interval (in minutes)
	* within which data must be received in order to keep the interface connected. If no data was received within
	* this interval, the interface will disconnect and wait-for-data again.
	*/
	uint32_t data_timeout;
	/** The type of IP address configuration */
	cg_net_ip_config_type_t	ip_config_type;
	/** IPv4 gateway */
	cg_ip_addr_t gw;
	/** First DNS server */
	cg_ip_addr_t dns1;
	/** Second DNS server */
	cg_ip_addr_t dns2;
} cg_net_wan_cfg_t;

/**
* \struct cg_net_cfg_t
* \brief Generic configuration settings for a network interface
*/
typedef struct {
	/** The interface IP address */
	cg_net_ip_config_t ip_config;
	/** The zone where this interface belongs to (WAN, LAN) */
	cg_net_zone_t zone;
	/** Configuration specific to the zone to which this interface belongs */
	union {
		cg_net_lan_cfg_t lan;
		cg_net_wan_cfg_t wan;
	} conf;
} cg_net_cfg_t;

/**
* \struct cg_net_if_t
* \brief Data structure describing a network interface in the system
*/
typedef struct {
	/** Name of the network interface, eg. eth0, uap0,... */
	char dev_name[CG_DEV_NAME_MAX];
	/** Interface status */
	cg_net_if_status_t status;
	/** The type of network interface */
	cg_net_if_type_t type;
	/** IP configuration for a WAN or LAN interface. */
	cg_net_cfg_t config;
} cg_net_if_t;

/**
* \enum cg_net_watchdog_action_t
* \brief Enumeration of the different watchdog actions.
*/
typedef enum {
	CG_CONN_WD_ACTION_RECONNECT = 0,
	CG_CONN_WD_ACTION_RESET
} cg_net_watchdog_action_t;

/**
* \struct cg_net_watchdog_settings_t
* \brief Settings structure for the connection watchdog
*/
typedef struct {
	/** Set to TRUE to enable connection watchdog. FALSE to disable */
	int enabled;
	/** The device will perform DNS lookups by default on addresses, unless an
	* IP address was specified. By setting this variable to TRUE, the device will
	* perform an additional ping.
	*/
	int use_ping;
	/** The frequency in seconds with which to perform the connection check. Eg. if an interval of
	* 60 seconds was specified, the connection test will be started every 60 seconds. TODO: specify minimum value
	* to cover maximum execution time of test.
	*/
	uint32_t interval;
	/** The action that the watchdog should undertake when it determines that there is no connectivity anymore */
	cg_net_watchdog_action_t action;
	/** The number of valid addresses in addresses */
	uint32_t num_addresses;
	/** Array of IP/host addresses */
	char addresses[CG_MAX_PING_ADDRESSES][CG_MAX_ADDRESS_LENGTH];
} cg_net_watchdog_settings_t;

/** Data type that contains a string of fixed length CG_DEV_NAME_MAX. Used in \ref cg_net_get_conn_priority_list and
* \ref cg_net_set_priority_conn_strat.
*/
typedef char cg_dev_name_t[CG_DEV_NAME_MAX];

/** Interface status callback type. Registered in \ref cg_net_register_interface_events.
* \param[in] dev_name The interface name for which the event is received.
* \param[in] status New \ref cg_net_if_status_t status bits
*/
typedef void (*itf_status_callback_t)(const char *dev_name, cg_net_if_status_t status);

/** Internet connection status callback. Registered in \ref cg_net_register_internet_events
* \param[in] status The current internet status.
*/
typedef void (*internet_status_callback_t)(cg_net_internet_status_t status);

/**
* \struct cg_net_stats_t
* \brief Structure that contains the data usage statistics for an interface
*/
typedef struct {
	/** The amount of received packets. */
	uint64_t rx_packets;
	/** The amount of received bytes. */
	uint64_t rx_bytes;
	/** The amount of transmitted packets. */
	uint64_t tx_packets;
	/** The amount of transmitted bytes. */
	uint64_t tx_bytes;
} cg_net_stats_t;

/** Get a list of interfaces in the system
* \param[out] num_interfaces Integer which will be set to the number of interfaces
* \param[out] interfaces *interfaces will be set to an array of cg_net_if_t. The number of elements in the array
* equal to num_interfaces. Caller needs to free the array.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_net_get_interface_list(uint32_t *num_interfaces, cg_net_if_t **interfaces);

/** Get a single interface in the system
* \param[in] dev_name The interface name for which to retrieve the interface info.
* \param[out] interface *interface will point to a cg_net_if_t object when the device is found, NULL otherwise.
* Caller needs to free the cg_net_if_t object.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter, CG_STATUS_RESOURCE_BUSY if a device is too cold or hot
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_net_get_interface(const char *dev_name, cg_net_if_t **interface);

/** Get a list of interfaces in the system from a certain type.
* \param[in] type The type of interface to list.
* \param[out] num_interfaces Integer which will be set to the number of interfaces
* \param[out] itfs *itfs will be set to an array of cg_net_if_t. The number of elements in the array
* equal to num_interfaces. Caller needs to free the array.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_net_get_interface_by_type(cg_net_if_type_t type, uint32_t *num_interfaces, cg_net_if_t **itfs);

/** Enable/disable a particular interface
* \param[in] dev_name The interface name for which the configuration is set. This is the dev_name member in the cg_net_if_t structure.
* \param[in] enabled Set to TRUE to enable the interface. Set to FALSE to disable interface.
* \return CG_STATUS_OK if successful. CG_STATUS_ERROR in case of error.
*/
cg_status_t cg_net_interface_set_enabled(const char *dev_name, int enabled);

/** Set the configuration for an interface
* \param[in] dev_name The interface name for which the configuration is set. This is the dev_name member in the cg_net_if_t structure.
* \param[in] config Caller-allocated configuration settings
* \return CG_STATUS_OK if successful,
CG_STATUS_INVALID_PARAMETER if dev_name or config contains incorrect data,
 CG_STATUS_ERROR if failed to set the new config, CG_STATUS_RESOURCE_BUSY if a device is too cold or hot.
*/
cg_status_t cg_net_set_config(const char *dev_name, const cg_net_cfg_t *config);

/** Register to receive events concerning interface state changes
* \param[in] dev_name The interface name for which to receive interface status changes
* \param[in] cb Callback which gets called whenever the interface changes status.
* \return CG_STATUS_OK if successful. CG_STATUS_ERROR in case of error.
*/
cg_status_t cg_net_register_interface_events(const char *dev_name, itf_status_callback_t cb);

/** Deregister to receive interface events.
* \param[in] dev_name The interface name for which to deregister callbacks.
*/
cg_status_t cg_net_deregister_interface_events(const char *dev_name);

/** Set the internet connection strategy to manual. If there was an active internet connection before, a new connection will
* be set up using the newly selected interface. Otherwise a separate connect call is necessary.
* \param[in] dev_name The WAN interface to use for internet connection.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER if dev_name contains an invalid device or
* CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_net_set_manual_conn_strat(const char *dev_name);

/** Get the interface which is used when using manual connection strategy. Returns an error when the current
* connection strategy is not manual.
* \param[out] dev_name Is filled in with the name of the interface that is being used to connect. Caller has to free dev_name.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER if dev_name is an invalid pointer or
*  CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_net_get_manual_conn_device(char **dev_name);

/** Set the internet connection strategy to priority
* \param[in] num_entries The number of interface names in priority_list
* \param[in] priority_list An array of cg_dev_name_t containing network interface names.
* The order in which the interface names are put in the array determines the priority. Priority decreases with array index increments.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER if num_entries or priority_list contains an invalid value or
*  CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_net_set_priority_conn_strat(uint32_t num_entries, cg_dev_name_t *priority_list);

/** Get the priority list which is used when using priority-based connection strategy. Returns error when
* the current connection strategy is not priority-based.
* \param[out] num_entries Is filled in with the number of entries in priority_list.
* \param[out] *priority_list is set to an array of cg_dev_name_t. The caller is responsible for freeing *priority_list.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER if num_entries or priority_list contains an invalid pointer or
*  CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_net_get_conn_priority_list(uint32_t *num_entries, cg_dev_name_t **priority_list);

/** Get the current configured connection strategy
* \param[out] strat Will be filled in with the current connection strategy.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER if strat is an invalid pointer or
*  CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_net_get_conn_strat( cg_net_conn_strat_t *strat);

/** Instructs the system to set up internet connection. Register for internet connection events
* to monitor the progress. If the device is configured for manual connection to a specific interface,
* the device will try to connect to that interface. If the device is configured for priority-based connection,
* the system will try to connect a WAN interface according to the priority list. This is a persistent setting across power cycles.
* \return CG_STATUS_OK if successful. CG_STATUS_ERROR in case no connection could be setup within 60 seconds although the
* device will keep on trying to connect.
*/
cg_status_t cg_net_connect(void);

/** Disconnection internet connection. This will return immediately & should put the internet connection state to CG_NET_INTERNET_DISCONNECTED.
* This is a persistent setting across power cycles.
* \return CG_STATUS_OK if successful. CG_STATUS_ERROR in case of error.
*/
cg_status_t cg_net_disconnect(void);

/** Returns the current internet connection state.
* \param[out] status Internet connection status
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_net_get_internet_status(cg_net_internet_status_t * status);

/** Register for internet connection status events. These are not interface-specific events, but events to
* describe the general internet connectivity status of the device.
* \return CG_STATUS_OK
*/
cg_status_t cg_net_register_internet_events(internet_status_callback_t cb);

/** Deregister for internet connection status events
* \return CG_STATUS_OK
*/
cg_status_t cg_net_deregister_internet_events(void);

/** Set the connection watchdog settings.
* If the enabled flag in the settings is set to FALSE, the connection watchdog will be disabled but the other settings will not be changed.
* \param[in] settings Watchdog settings
* \return CG_STATUS_OK if successful. CG_STATUS_ERROR in case of error. CG_STATUS_INVALID_PARAMETER in case of wrong parameters.
*/
cg_status_t cg_net_set_watchdog(cg_net_watchdog_settings_t *settings);

/** Query the connection watchdog settings
* \param[out] settings Double pointer to a structure that will be allocated and filled in with the connection watchdog settings.
* \return CG_STATUS_OK if successful, CG_STATUS_ERROR in case of error or CG_STATUS_INVALID_PARAMETER if setting is a null pointer.
*/
cg_status_t cg_net_get_watchdog(cg_net_watchdog_settings_t **settings);

/** Start or stop data usage tracking for a specific interface. This setting will be persistent across reboots.
* Enabling, even when already enabled, ensures all usage tracking data is saved to flash at least every 10 minutes.
* This more frequent writing is not persistent across reboot.
* Disabling will cause the "Data Counters" section on the CloudGate webpage for this interface to stop.
* \param[in] dev_name The interface name for which the data counter needs to be enabled or disabled.
* \param[in] enabled TRUE if data usage tracking needs to be enabled for this interface, FALSE if it needs to be disabled.
* \return CG_STATUS_OK if successful, CG_STATUS_ERROR when an unexpected situation arises
* and CG_STATUS_INVALID_PARAMETER if dev_name is a NULL pointer or if dev_name does not exist.
*/
cg_status_t cg_net_datacounter_set_enabled(const char *dev_name, int enabled);

/** Get the data usage tracking state for a specific interface.
* \param[in] dev_name The interface name for which the data usage tracking state needs to be retrieved.
* \param[out] enabled TRUE if data usage tracking is enabled for this interface, FALSE if disabled.
* \return CG_STATUS_OK if successful, CG_STATUS_ERROR when an unexpected situation arises
* , CG_STATUS_INVALID_PARAMETER if dev_name or enabled is a NULL pointer or if dev_name does not exist,
* CG_STATUS_RESOURCE_BUSY if a device is too cold or hot.
*/
cg_status_t cg_net_datacounter_get_enabled(const char *dev_name, int *enabled);

/** Reset the data usage statistics for a specific interface.
* \param[in] dev_name The interface name for which the data counter needs to be reset.
* \return CG_STATUS_OK if successful, CG_STATUS_ERROR when an unexpected situation arises
* and CG_STATUS_INVALID_PARAMETER if dev_name is a NULL pointer or if dev_name does not exist.
*/
cg_status_t cg_net_datacounter_reset_stats(const char *dev_name);

/** Get the data usage statistics for a specific interface.
* \param[in] dev_name The interface name for which the data usage statistics need to be retrieved.
* \param[out] stats Caller-allocated struct that will be filled with current data usage statistics for this interface.
* \return CG_STATUS_OK if successful, CG_STATUS_ERROR when an unexpected situation arises,
* CG_STATUS_INVALID_PARAMETER if dev_name or stats is a NULL pointer or if dev_name does not exist,
* CG_STATUS_RESOURCE_BUSY if a device is too cold or hot.
*/
cg_status_t cg_net_datacounter_get_stats(const char *dev_name, cg_net_stats_t *stats);

/** Set a new MTU for a specific interface.
* \param[in] dev_name The interface name for which the MTU needs to be set.
* \param[in] mtu The new MTU value, this must be a number from 68 to 1500.
* \return CG_STATUS_OK if successful,
CG_STATUS_INVALID_PARAMETER if dev_name or mtu contains incorrect values,
CG_STATUS_ERROR if failed to set the new MTU,
CG_STATUS_RESOURCE_BUSY if a device is too cold or hot.
*/
cg_status_t cg_net_set_mtu(const char *dev_name, int mtu);

/** Get the current MTU for a specific interface.
* \param[in] dev_name The interface name for which the MTU needs to be retrieved.
* \param[out] mtu The current MTU value
* \return CG_STATUS_OK if successful,
CG_STATUS_INVALID_PARAMETER if dev_name or mtu contains incorrect values,
CG_STATUS_ERROR if failed to get the current MTU, CG_STATUS_RESOURCE_BUSY if a device is too cold or hot.
*/
cg_status_t cg_net_get_mtu(const char *dev_name, int *mtu);

/** Set a device disabled at boot. This will allow software to take full control of a device before enabling it.
* The device will always start disabled and must be started by cg_net_interface_set_enabled()
* \param[in] dev_name The interface name for which the data counter needs to be reset.
* \param[in] disabled TRUE: interface will be disabled at boot, FALSE: current interface state will remain unmodified
* \return CG_STATUS_OK if successful, CG_STATUS_ERROR when an unexpected situation arises
* and CG_STATUS_INVALID_PARAMETER if dev_name is a NULL pointer or if dev_name does not exist.
* Note: When clearing this feature the interface will not become enabled automatically. It will fall back to the last known state.
*/
cg_status_t cg_net_set_disabled_at_boot(const char *dev_name, int disabled );

/** Get the device disabled at boot status.
* The device will always start disabled and must be started by cg_net_interface_set_enabled()
* \param[in] dev_name The interface name for which the data counter needs to be reset.
* \param[out] disabled TRUE: interface will be disabled at boot, FALSE: current interface state will remain unmodified
* \return CG_STATUS_OK if successful, CG_STATUS_ERROR when an unexpected situation arises
* and CG_STATUS_INVALID_PARAMETER if dev_name is a NULL pointer or if dev_name does not exist.
*/
cg_status_t cg_net_get_disabled_at_boot(const char *dev_name, int *disabled );
/** @} */

CG_END_DECLS

#endif /* __CG_NET_H__ */
