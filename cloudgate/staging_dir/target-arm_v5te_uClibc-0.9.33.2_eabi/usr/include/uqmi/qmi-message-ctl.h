
#include "qmi-message.h"

struct qmi_ctl_set_instance_id_request {
	struct {
		unsigned int id : 1;
	} set;
	struct {
		uint8_t id;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_ctl_set_instance_id_response {
	struct {
		unsigned int link_id : 1;
	} set;
	struct {
		uint16_t link_id;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_ctl_get_version_info_response {
	struct {
	} set;
	struct {
		unsigned int service_list_n;
		struct {
			QmiService service;
			uint16_t major_version;
			uint16_t minor_version;
		} *service_list;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_ctl_allocate_cid_request {
	struct {
		unsigned int service : 1;
	} set;
	struct {
		QmiService service;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_ctl_allocate_cid_response {
	struct {
		unsigned int allocation_info : 1;
	} set;
	struct {
		struct {
			QmiService service;
			uint8_t cid;
		} allocation_info;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_ctl_release_cid_request {
	struct {
		unsigned int release_info : 1;
	} set;
	struct {
		struct {
			QmiService service;
			uint8_t cid;
		} release_info;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_ctl_release_cid_response {
	struct {
		unsigned int release_info : 1;
	} set;
	struct {
		struct {
			QmiService service;
			uint8_t cid;
		} release_info;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_ctl_set_data_format_request {
	struct {
		unsigned int format : 1;
		unsigned int protocol : 1;
	} set;
	struct {
		QmiCtlDataFormat format;
		QmiCtlDataLinkProtocol protocol;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_ctl_set_data_format_response {
	struct {
		unsigned int protocol : 1;
	} set;
	struct {
		QmiCtlDataLinkProtocol protocol;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_ctl_internal_proxy_open_request {
	struct {
	} set;
	struct {
		char *device_path;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

int qmi_set_ctl_set_instance_id_request(struct qmi_msg *msg, struct qmi_ctl_set_instance_id_request *req);
int qmi_parse_ctl_set_instance_id_response(struct qmi_msg *msg, struct qmi_ctl_set_instance_id_response *res);
void qmi_cleanup_ctl_set_instance_id_response(struct qmi_ctl_set_instance_id_response *res);

int qmi_set_ctl_get_version_info_request(struct qmi_msg *msg);
int qmi_parse_ctl_get_version_info_response(struct qmi_msg *msg, struct qmi_ctl_get_version_info_response *res);
void qmi_cleanup_ctl_get_version_info_response(struct qmi_ctl_get_version_info_response *res);

int qmi_set_ctl_allocate_cid_request(struct qmi_msg *msg, struct qmi_ctl_allocate_cid_request *req);
int qmi_parse_ctl_allocate_cid_response(struct qmi_msg *msg, struct qmi_ctl_allocate_cid_response *res);
void qmi_cleanup_ctl_allocate_cid_response(struct qmi_ctl_allocate_cid_response *res);

int qmi_set_ctl_release_cid_request(struct qmi_msg *msg, struct qmi_ctl_release_cid_request *req);
int qmi_parse_ctl_release_cid_response(struct qmi_msg *msg, struct qmi_ctl_release_cid_response *res);
void qmi_cleanup_ctl_release_cid_response(struct qmi_ctl_release_cid_response *res);

int qmi_set_ctl_set_data_format_request(struct qmi_msg *msg, struct qmi_ctl_set_data_format_request *req);
int qmi_parse_ctl_set_data_format_response(struct qmi_msg *msg, struct qmi_ctl_set_data_format_response *res);
void qmi_cleanup_ctl_set_data_format_response(struct qmi_ctl_set_data_format_response *res);

int qmi_set_ctl_sync_request(struct qmi_msg *msg);
int qmi_parse_ctl_sync_response(struct qmi_msg *msg);
int qmi_set_ctl_internal_proxy_open_request(struct qmi_msg *msg, struct qmi_ctl_internal_proxy_open_request *req);
int qmi_parse_ctl_internal_proxy_open_response(struct qmi_msg *msg);
