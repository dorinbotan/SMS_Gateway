#ifndef __CG_UPGRADE_H__
#define __CG_UPGRADE_H__
#include <stdint.h>
CG_BEGIN_DECLS

/**
* \file cg_upgrade.h
* \brief Header file containing definitions for upgrade API
*/

/**
* \defgroup prov Upgrade API
* @{
*/

/** handle for upgrade API
* \note
*  created by cg_uprade_start, used by cg_upgrade_data & cg_upgrade_stop
*/
typedef struct cg_upgrade cg_upgrade_t;

/** Prepares a device upgrade
*  \note
*  Switch off auto upgrade by the provisioning server to be sure the manual provisioning will not be overruled by an automatic provisioning from the provisioning server.
*  After a successful call of cg_upgrade_start, call cg_upgrade_data to transfer data and finish the upgrade by calling cg_upgrade_stop, even if the upgrading process is stopped by a failing upgrade data transfer.
*  \param[out] handle: handle needed by other upgrade functions
* \return CG_STATUS_OK if function  call was successful, CG_STATUS_ERROR in case of error, CG_INVALID_PARAMETER if handle is NULL, CG_STATUS_RESOURCE_BUSY if there is an ongoing device upgrade busy
*/
cg_status_t cg_upgrade_start(cg_upgrade_t **handle);

/** Performs the upgrade based on data chunks\n
* Once the last chunk of data has been successfully received by the gateway, the new image is commited and can't be reversed.\n
* cg_upgrade_stop() indicates whether the upgrade was successful.
*  \note
* Please limit the size of the data chunks to 16384 bytes, in order not to exhaust memory resources during upgrade.
* \param[in] handle: handle returned by cg_upgrade_start
* \param[in] buffer: character buffer with a chunk of data
* \param[in] length: actual length of that chunk of data in bytes
* \return CG_STATUS_OK if no errors were encountered, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter submitted, CG_STATUS_ERROR in case an upgrade failure was encountered
*/
cg_status_t  cg_upgrade_data( cg_upgrade_t *handle, char *buffer, uint32_t len);

/** Destroys internal context and returns the overall status of the device upgrade\n
* If the upgrade was successful then the gateway will boot with the new image after a reset.\n
* cg_reset_system() can be called to reboot the gateway.
* \param[in] handle: handle returned by cg_upgrade_start
* \return CG_STATUS_OK if device upgrade was successful, CG_STATUS_INVALID_PARAMETER when a parameter is invalid, CG_STATUS_ERROR if device upgrade errors were encountered
*/
cg_status_t cg_upgrade_stop( cg_upgrade_t *handle);

/** @} */

CG_END_DECLS

#endif /* __CG_UPGRADE_H__*/
