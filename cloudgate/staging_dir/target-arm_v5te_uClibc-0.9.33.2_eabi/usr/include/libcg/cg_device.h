#ifndef __CG_DEVICE_H__
#define __CG_DEVICE_H__

#include <libcg/cgate.h>

CG_BEGIN_DECLS

/**
* \file cg_device.h
* \brief Header file containing definition of all device management related function calls
*/

/**
* \note TODO:
*		+ Registering/deregistering of board definitions ?
*/

/**
* \defgroup devmgmt Device Management
* @{
*/

/**
* \enum cg_device_status_t Various device status flags
*/
typedef enum {
	/** Device is physically present in the system */
	CG_DEVICE_PRESENT = 0x1,
	/** Device has been claimed by a process */
	CG_DEVICE_CLAIMED = 0x2,
} cg_device_status_t;

/**
* \enum cg_device_type_t List of possible device types
*/
typedef enum {
	/** Network interface (WLAN/LAN/3G/...) */
	CG_DEVTYPE_NETWORK = 0x1,
	/** Serial port */
	CG_DEVTYPE_SERIAL = 0x2,
	/**  Digital-Analog Converter */
	CG_DEVTYPE_DAC = 0x4,
	/** GPIO pin */
	CG_DEVTYPE_GPIO = 0x8,
	/** Analog-Digital Converter */
	CG_DEVTYPE_ADC = 0x10,
} cg_device_type_t;

/**
* \enum cg_location_t List of possible locations
*/
typedef enum {
	/** The location is unknown */
	CG_LOC_UNKNOWN = 0x0,
	/** The first slot, located below the front side with the leds */
	CG_LOC_SLOT1 = 0x1,
	/** The second slot, located below the back side with the power supply */
	CG_LOC_SLOT2 = 0x2,
	/** Located on the mainboard */
	CG_LOC_MAIN = 0x4,
	/** All locations */
	CG_LOC_ALL = 0x7
} cg_location_t;

/**
* \struct cg_device_t
* \brief Basic properties of a hardware device present in the system
*/
typedef struct {
	/** A user-friendly name for the hardware device, e.g. "Gobi 0" */
	char friendly_name[52];
	/** 
	*  The actual device path in order to access the device, e.g. "/dev/qmi0".
	*  For network devices this is the name as can be used with the cg_wwan and
        *  cg_net SDK functions.	
	*/
	char device_name[100];
	/** The type of device */
	cg_device_type_t device_type;
	/** The location of the device */
	cg_location_t device_location;
	/** Indicates various device status flags*/
	cg_device_status_t status;
}cg_device_t;

/**
* \struct cg_board_t
* \brief Description of a pluggable expansion board
*/
typedef struct {
	/** The slot in which the board is inserted */
	uint8_t slot_id;
	/** The ID of the board as read out from the EEPROM */
	char	board_id[15];
	/** The hardware version of the expansion board */
	char	hw_ver[5];
} cg_board_t;

/**
* \enum cg_device_event_t List of all possible device events
*/
typedef enum {
	/** Event to indicate that a device has arrived */
	CG_DEV_EVT_ARRIVED,
	/** Event to indicate that a device has been removed */
	CG_DEV_EVT_REMOVED,
	/** Event to indicate that a device has been claimed by a process */
	CG_DEV_EVT_CLAIMED,
	/** Event to indicate that a device has been released by a process and can now be claimed again */
	CG_DEV_EVT_RELEASED
} cg_device_event_t;


/** Device notification callback
* \param[in] device Device for which notification is received.
* \param[in] evt Received event
* \param[in] context The context parameter which was passed on to \ref cg_device_register_notification
*/
typedef void (*device_notification_cb_t)(cg_device_t *device, cg_device_event_t evt, void *context);

/** Device claim callback
* \param[in] device The device for which a claim request is received.
* \param[in] context The context parameter that was passed on to \ref cg_device_claim
* \return CG_STATUS_OK to release the device. CG_STATUS_ERROR to hold on to the device.
*/
typedef cg_status_t (*device_claim_cb_t)(cg_device_t *device, void *context);


/** Returns a list of present accessible hardware devices
* \param[in] device_mask A mask of cg_device_type_t values to indicate the types of devices to list
* \param[out] num_devices Number of device structures returned
* \param[out] devices List of device structures allocated by callee and assigned to *devices
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of invalid parameter
* (e.g. device_mask cannot be '0') or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_device_list(uint32_t device_mask, uint32_t *num_devices, cg_device_t **devices);

/** Returns a list of present accessible hardware devices at a given location
* \param[in] location_mask A mask of cg_location_t values to indicate the locations of the devices to list
* \param[in] device_mask A mask of cg_device_type_t values to indicate the types of devices to list
* \param[out] num_devices Number of device structures returned
* \param[out] devices List of device structures allocated by callee and assigned to *devices
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of invalid parameter
* (e.g. device_mask cannot be '0') or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_device_list_by_location(uint32_t location_mask, uint32_t device_mask, uint32_t *num_devices, cg_device_t **devices);

/** Marks the device as used by the calling process. It is possible that the device is currently being used
* by another entity. In that case that entity is asked to release the device. If the entity agrees, the device
* is assigned to the caller. If the entity does not agree, an error is returned and you cannot claim the device.
* On the other hand: if another entity wants to use the device, the callback cb is called. If cb returns
* CG_STATUS_OK, it indicates that you allow to release the device so the other entity can claim it. When
* returning CG_STATUS_ERROR, you indicate that you wish to keep hold of the device.
* \param[in] device The device to claim
* \param[in] cb Callback when another device wants to use the device you are using right now. return CG_STATUS_OK
* if you agree to release the device. Return CG_STATUS_ERROR if you want to keep on using the device.
* If you do not provide a callback, all other claims will be responded with CG_STATUS_ERROR.
* \param[in] context Context parameter passed along to cb when called.
* \return CG_STATUS_OK in case of success. CG_STATUS_RESOURCE_BUSY when the device is kept in use.
* CG_STATUS_INVALID_PARAMETER for invalid parameters (e.g. device = NULL), CG_STATUS_ERROR otherwise.
*/
cg_status_t cg_device_claim(cg_device_t *device, device_claim_cb_t cb, void *context);

/** Releases the device so it is free to be used by another process
* \param[in] device The device to release
* \return CG_STATUS_OK in normal case. GG_STATUS_INVALID_PARAMETER for invalid parameter (e.g. device == NULL), CG_STATUS_ERROR otherwise.
*/
cg_status_t cg_device_release(cg_device_t *device);

/** Registers a callback for notifications about device types. Calling this function multiple times
* will cause the previous registration to be overwritten.
* \param[in] device_mask A mask of cg_device_type_t types.
* \param[in] cb This callback will be called when a notification is received.
* \param[in] context A parameter which will be passed on to the callback when it is called.
* \return CG_STATUS_OK if successful or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_device_register_notification(uint32_t device_mask, device_notification_cb_t cb, void *context);

/** Deregisters a callback for notifications about a specific device or device type.
* \return CG_STATUS_OK if successful of CG_STATUS_ERROR in case of error.
*/
cg_status_t cg_device_deregister_notification(void);

/** Requests a list of expansion boards currently plugged into the system
* \param[out] num_boards Gets filled in by callee with the number of boards currently inserted.
* \param[out] boards List of boards in the system. Callee will allocate array of boards and will assign it to *boards. Caller needs to free the
* the array.
* \return CG_STATUS_OK if successful of CG_STATUS_ERROR in case of error.
*/
cg_status_t cg_device_board_list(uint8_t *num_boards, cg_board_t **boards);

/** Requests a list of devices available on an expansion board currently plugged into the system
 * \param[in]  board details of board the caller want the device list from
 * \param[out] num_devices Gets filled in by callee with the number of devices on the board.
 * \param[out] devices List of devices on a boards in the system. Callee will allocate array of devices and will assign it to *devices. Caller needs to free the
 * the array.
 * \return CG_STATUS_OK if successful of CG_STATUS_ERROR in case of error.
 */
cg_status_t cg_device_board_device_list(cg_board_t* board, uint32_t *num_devices, cg_device_t **devices);


/** @} */

CG_END_DECLS

#endif /* __CG_DEVICE_H__ */
