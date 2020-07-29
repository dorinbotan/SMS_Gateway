#ifndef LIBMODEMIMG_H
#define LIBMODEMIMG_H

#include <evotils/list.h>
#include <mcmd/mcm_messages.h>
#include <stdint.h>

/** The firmware is built for a specific carrier */
typedef enum {
	MODEM_CARRIER_UNKNOWN,
	MODEM_CARRIER_GENERIC,
	MODEM_CARRIER_VERIZON,
	MODEM_CARRIER_ATT
} modem_carrier_t;

/** The main revision of the firmware */
typedef struct {
	uint32_t major;
	uint32_t minor;
} modem_fw_revision_t;

/** The (optional) secondary revision of the firmware */
typedef struct {
	uint32_t major;
	uint32_t middle;
	uint32_t minor;
} modem_fw_arn_revision_t;

/** Struct representing current image on module. */
typedef struct {
	modem_carrier_t carrier; //e.g. Verizon
	modem_fw_revision_t rev; //Main revision
	modem_fw_arn_revision_t arn; //Optional revision.
} modem_fw_info_t;

/* General return values: 0 = success, 1 = failure */
/*
 * Initialize the library.
 * \param[in]: dynmount. Absolute path to mount point. Must be seen in /etc/fstab or /etc/mtab. Example: /mnt/data
 * \param[in]: dynroot. The root directory for firmware images. Child directory of dynmount.
 * \param[in]: dynblobs. Directory for the raw firmware images. Child directory of dynroot. If NULL, 'blobs' will be used by default.
 * \param[in]: dynuci. Directory for the UCI files. Child directory of dynroot. If NULL, 'uci' will be used by default.
 */
int libmodem_img_init(const char* dynmount, const char *dynroot, const char *dynblobs, const char *dynuci);
int libmodem_img_deinit();
int libmodem_set_current_modem_via_fw_type(const char *fw_type);
int libmodem_set_current_modem_via_cfg_type(const char *cfg_type);
int libmodem_set_current_modem(modem_type_t modem_type);

inline char* get_path_to_modem_images();
inline char* get_path_to_modem_config();
inline char* get_path_to_modem_uci();
inline char* get_path_to_modem_uci_file();
inline char* get_path_to_modem_uci_misc_file();

char* modem_read_image_family(const char *id);
char* modem_read_fw_bid(const char *id);
char* modem_read_fw_file(const char *id);
char* modem_get_image_id_on_fw_bid(const char *fw_bid);
/* Checks the sha256 checksum of the modme image. Return: 1 = valid, 0 = invalid */
int modem_image_checksum_valid(const char *id);
int modem_image_is_upgrade_required(modem_fw_info_t *fw_info, modem_fw_info_t *fs_fw_info, int compare_rev, int compare_arev);

int libmodem_img_conf_misc_get( const char *var, char **value_p);
int libmodem_img_conf_misc_set( const char *var, const char *value);

list_t* modem_get_image_list();
hashtable_t *modem_get_needed_uids();
int modem_rm_image_file(const char* id);
int modem_cleanup_blobs();
int32_t modem_prov_blob_handler(const char* type, const char* uuid);
uint32_t modem_set_fw_info(const char *firmware_info, modem_fw_info_t *fw_info);

int32_t modem_prov_blob_handler(const char* type, const char* uuid);

#endif
