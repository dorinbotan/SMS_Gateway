#ifndef __WLAN_SNIFFER_LOG_H__
#define __WLAN_SNIFFER_LOG_H__
typedef struct wifimeasurement{
	u_char	bssid[6];
	//char	rssi_dbm;
	signed int	rssi_dbm;
	struct timeval ts;
} wifimeasurement_t;

int macaddress_compare (void * macaddress2compare2, void * macaddress2becompared);
void inittree (void *);
int isblacklisted (const u_char *bssid);
//int logtotree (const struct timeval *ts,const u_char *bssid,char rssi_dbm, int channel_frequency );
void logtotree (void *arg);
void treetologfile (void *arg);
void binary_tree_stop(void *arg);
void binary_tree_start(void *arg);
void binary_tree_set_file_timestamp(void *arg);

BinaryTree *binary_tree_get(void);
BinaryTreeEnum *binary_tree_enum_get(void);

int binary_tree_get_height(void);

void binary_tree_setdefaultlogfilename(wlan_sniffer_ctx_t *ctx);
char *binary_tree_get_log_file(wlan_sniffer_ctx_t *ctx);
char *binary_tree_get_log_file_small(wlan_sniffer_ctx_t *ctx);
void binary_tree_start(void *arg);
void binary_tree_stop(void *arg);

#endif // __WLAN_SNIFFER_LOG_H__
