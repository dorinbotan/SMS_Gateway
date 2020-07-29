#ifndef __CG_WLAN_SNIFFER_H__
#define __CG_WLAN_SNIFFER_H__
#include <libcg/cgate.h>

CG_BEGIN_DECLS

/**
* \file cg_wlan_sniffer.h
* \brief Header file containing all definitions/functions related to wlan sniffing. 
*/

/**
* \defgroup wlan_sniffer WLAN_SNIFFER
* @{
*/

/**
* \enum cg_wlan_sniffer_params_t
* \brief Input parameters for WLAN sniffer
*/

/** Maximum length of the string containing the file monitoring path */
#define CG_MONITORING_FILE_PATH_MAX_LEN 64

typedef struct cg_wlan_sniffer_params {
	/** window size in seconds */
	uint32_t window_size;
	/** wifi channel number */
	uint32_t channel;
	/** max file size in bytes */
	uint32_t max_file_size;
	/** maximum number of files created */
	uint32_t max_nbr_files;
	/** monitoring file path */
	char monitoring_file_path[CG_MONITORING_FILE_PATH_MAX_LEN+1];
	/** detailed or simple monitoring */
	uint32_t detailed_monitoring;
} cg_wlan_sniffer_params_t;


/** Initiate wlan sniffing.
* The function sets the supplied parameters and starts wlan logging probe requests for a time span
* given by the window size. The logging result is filtered and written to an output file. As soon
* the size of the output file reaches "max_file_size", the file is closed and a new file with a 
* different name is created. The maximum number of files created is given by the parameter "max_nbr_files".
* \param[in] window_size: value in seconds.
* \param[in] channel: wifi channel to log.
* \param[in] max_file_size: maximum file size in bytes.
* \param[in] max_nbr_files: maximum number of files created.
* \param[in] monitoring_file_path: file path for monitoring log files.
* \param[in] detailed_monitoring: 0 simple monitoring , 1 detailed monitoring.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_sniffer_start(uint32_t window_size,uint32_t channel,uint32_t max_file_size, uint32_t max_nbr_files,const char *monitoring_file_path, uint32_t detailed_monitoring);
/** stop wlan sniffing.
* The functions stops wlan logging and stops also wlan_snifferd daemon.
* \return CG_STATUS_OK if successful, CG_STATUS_INVALID_PARAMETER in case of an invalid parameter
* or CG_STATUS_ERROR in case of an error.
*/
cg_status_t cg_wlan_sniffer_stop(void);

/** @} */

CG_END_DECLS

#endif /* __CG_WLAN_SNIFFER_H__ */
