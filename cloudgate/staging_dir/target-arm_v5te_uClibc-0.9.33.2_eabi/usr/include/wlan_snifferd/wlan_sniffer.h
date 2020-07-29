#ifndef WLAN_SNIFFER_H
#define WLAN_SNIFFER_H
#include <libcg/cgate.h>
#include <signal.h>
#include <evotils/list.h>
#include <pcap.h> // for all PCAP functions

#define wlan_snifferd_log(x, y, ...) log(LOG_WLAN_SNIFFERD, x, y, ##__VA_ARGS__)

#define WLAN_SNIFFERD_VERSION 8
#define MAX_LENGTH_FILE_NAME 64
#define DEFAULT_LOG_FILE_NAME_SMALL "wlansnifferlog_small"
#define DEFAULT_LOG_FILE_NAME       "wlansnifferlog"
#define DEFAULT_LOG_FOLDER_NAME     "/tmp"
#define DEFAULT_WLANMON_FOLDER_NAME "/tmp/wlanmon"


typedef enum {
	WLAN_SNIFFER_ERR_NO_DEVICE_AVAILABLE = CG_STATUS_RESOURCE_UNAVAILABLE,
	WLAN_SNIFFER_ERR_TRACE_NO_CONTEXT,
	WLAN_SNIFFER_ERR_TRACE_PCAP_OPEN_ERROR,
	WLAN_SNIFFER_ERR_TRACE_PCAP_COMPILE_ERROR,
	WLAN_SNIFFER_ERR_TRACE_FILTER_ERROR,
	WLAN_SNIFFER_ERR_TRACE_THREAD_CREATE_ERROR,
	WLAN_SNIFFER_ERR_TRACE_BINARYTREE_CREATE_ERROR
} wlan_sniffer_error_t;

typedef struct wlan_sniffer_device_list {
	int nbr_devices;
	char device_name[1][MAX_DEV_NAME_LEN];
} wlan_sniffer_device_list_t;

typedef struct parameters {
	uint32_t window_size;
	uint32_t channel;
	uint32_t max_file_size;
	uint32_t max_nbr_files;
	char monitoring_file_path[MAX_LENGTH_FILE_NAME+1];
	uint32_t detailed_monitoring;
	} wlan_sniffer_parameters_t;

/** called when a wifi probe request is detected
 */
typedef void (*callbackfunc_t) (char *device_name, const struct timeval *ts,const u_char *bssid,signed int rssi_dbm, int channel_frequency, int cancel, wlan_sniffer_parameters_t *parameters );

typedef struct wlan_sniffer_timer {
	struct sigevent         te;
	struct itimerspec       its;
	struct sigaction        sa;
	/** timer id  initialised by timer_create */
	timer_t 		timer_id;
	/** indicates timer is set */
	int timer_set;
} wlan_sniffer_timer_t;


typedef struct wlan_sniffer_ctx {
	/** list */
	list_t list;
	/** libpcap handle */
	pcap_t *handle;
	/** user defined callbackfunc */
	callbackfunc_t callbackfunc;
	/** thread id */
	pthread_t tid;
	/** device name sniffer */
	char device_name[MAX_DEV_NAME_LEN];
	/** user supplied parameters */
	wlan_sniffer_parameters_t parameters;
	/** libpcap buffer to compile filter */
	struct bpf_program fp;
	/** libpcap timeout timer context */
	wlan_sniffer_timer_t timer;
	/** indicates thread running */
	int thread_running;
	/** indicates wlan interface initialized */
	int init_if_done;
	/** log file name with timestamp (detailed logger)*/
	char current_log_file_name[MAX_LENGTH_FILE_NAME+1];
	/** log file name with timestamp (simple logger)*/
	char current_log_file_name_small[MAX_LENGTH_FILE_NAME+1];
	/** folder containing temporary log file */
	char log_folder_name[MAX_LENGTH_FILE_NAME+1];
	/** logging folder that is checked by serial framework */
	char wlanmon_folder_name[MAX_LENGTH_FILE_NAME+1];
	/** default log file name ( detailed logger ) */
	char default_log_file_name[MAX_LENGTH_FILE_NAME+1];
	/** default log file name ( simple logger ) */
	char default_log_file_name_small[MAX_LENGTH_FILE_NAME+1];
	/** cached channel */
	int cached_channel;
	/** set to 1 by wlan_snifferd_stop */
	int stopped;
	/** context lock */
	pthread_mutex_t lock;
	/** nbr traces since start */
	int nbr_traces;
} wlan_sniffer_ctx_t;

/** inits the logging devices
 * \param[ in ] device_name: wifi device to log
 * \param[ in ] channel: wifi channel to log
 * \param[ in ] value of previous channel setting
 * \return 1 if interface needs to be reinitialised
 */
int wlan_sniffer_init_device(char *device, u_int channel,u_int previous_channel);

/** dinits the logging devices
 * \param[ in ] devic_name
 */
void wlan_sniffer_deinit_all_devices(void);
/** starts wlan logging
 * \param[ in ] callbackfunc: supplied function is called wlan a wifi probe request is detected
 */
int wlan_sniffer_trace(char *dev, callbackfunc_t callbackfunc);

/** sets parameters
 * \param[ in ] devic_name
 * \param[ in ] window_size time span logging in seconds
 * \param[ in ] channel: wifi channel to log
 * \param[ in ] max_nbr_files: maximum number of files to writte logging information
 * \param[ in ] max_file_size: maximum file size in bytes
 * \param[ in ] monitoring_file_path: file path for monitoring log files
 * \param[ in ] detailed_monitoring: 0 simple monitoring, 1 detailed monitoring
 * \return -EINVAL if invalid parameters, -ENOMEM if no memory and 0 if success 
 */
int wlan_sniffer_set_parameters(char *device_name,unsigned int window_size,unsigned int channel,unsigned int max_nbr_files, unsigned int max_file_size,const char *monitoring_file_path, unsigned int detailed_monitoring);

/** Returns a list of present TP-Link devices
 * \param[ out ] device_list the list of devices allocated by callee, caller must free the pointer
 * \return the number of detected devices if success,
 */
int wlan_sniffer_search_devices(wlan_sniffer_device_list_t **device_list);

/** Retrieves context of a sniffer device
 * \return context
 */
wlan_sniffer_ctx_t *wlan_sniffer_get_ctx(char *device_name);
#endif
