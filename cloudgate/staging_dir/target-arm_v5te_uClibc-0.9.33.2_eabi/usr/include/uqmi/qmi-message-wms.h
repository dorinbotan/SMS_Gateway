
#include "qmi-message.h"

struct qmi_wms_set_event_report_request {
	struct {
		unsigned int new_mt_message_indicator : 1;
	} set;
	struct {
		struct {
			bool report;
		} new_mt_message_indicator;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_raw_send_request {
	struct {
		unsigned int raw_message_data : 1;
		unsigned int cdma_force_on_dc : 1;
		unsigned int cdma_follow_on_dc : 1;
		unsigned int gsm_wcdma_link_timer : 1;
		unsigned int sms_on_ims : 1;
	} set;
	struct {
		struct {
			QmiWmsMessageFormat format;
			unsigned int raw_data_n;
			uint8_t *raw_data;
		} raw_message_data;
		struct {
			bool force;
			QmiWmsCdmaServiceOption service_option;
		} cdma_force_on_dc;
		struct {
			bool follow;
		} cdma_follow_on_dc;
		uint8_t gsm_wcdma_link_timer;
		bool sms_on_ims;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_raw_send_response {
	struct {
		unsigned int message_id : 1;
		unsigned int cdma_cause_code : 1;
		unsigned int cdma_error_class : 1;
		unsigned int gsm_wcdma_cause_info : 1;
		unsigned int message_delivery_failure_type : 1;
	} set;
	struct {
		uint16_t message_id;
		QmiWmsCdmaCauseCode cdma_cause_code;
		QmiWmsCdmaErrorClass cdma_error_class;
		struct {
			QmiWmsGsmUmtsRpCause rp_cause;
			QmiWmsGsmUmtsTpCause tp_cause;
		} gsm_wcdma_cause_info;
		QmiWmsMessageDeliveryFailureType message_delivery_failure_type;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_raw_write_request {
	struct {
		unsigned int raw_message_data : 1;
	} set;
	struct {
		struct {
			QmiWmsStorageType storage_type;
			QmiWmsMessageFormat format;
			unsigned int raw_data_n;
			uint8_t *raw_data;
		} raw_message_data;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_raw_write_response {
	struct {
		unsigned int memory_index : 1;
	} set;
	struct {
		uint32_t memory_index;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_raw_read_request {
	struct {
		unsigned int message_memory_storage_id : 1;
		unsigned int message_mode : 1;
		unsigned int sms_on_ims : 1;
	} set;
	struct {
		struct {
			QmiWmsStorageType storage_type;
			uint32_t memory_index;
		} message_memory_storage_id;
		QmiWmsMessageMode message_mode;
		bool sms_on_ims;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_raw_read_response {
	struct {
		unsigned int raw_message_data : 1;
	} set;
	struct {
		struct {
			QmiWmsMessageTagType message_tag;
			QmiWmsMessageFormat format;
			unsigned int raw_data_n;
			uint8_t *raw_data;
		} raw_message_data;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_modify_tag_request {
	struct {
		unsigned int message_tag : 1;
		unsigned int message_mode : 1;
	} set;
	struct {
		struct {
			QmiWmsStorageType storage_type;
			uint32_t memory_index;
			QmiWmsMessageTagType message_tag;
		} message_tag;
		QmiWmsMessageMode message_mode;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_delete_request {
	struct {
		unsigned int memory_storage : 1;
		unsigned int memory_index : 1;
		unsigned int message_tag : 1;
		unsigned int message_mode : 1;
	} set;
	struct {
		QmiWmsStorageType memory_storage;
		uint32_t memory_index;
		QmiWmsMessageTagType message_tag;
		QmiWmsMessageMode message_mode;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_get_message_protocol_response {
	struct {
		unsigned int message_protocol : 1;
	} set;
	struct {
		QmiWmsMessageProtocol message_protocol;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_list_messages_request {
	struct {
		unsigned int storage_type : 1;
		unsigned int message_tag : 1;
		unsigned int message_mode : 1;
	} set;
	struct {
		QmiWmsStorageType storage_type;
		QmiWmsMessageTagType message_tag;
		QmiWmsMessageMode message_mode;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_list_messages_response {
	struct {
	} set;
	struct {
		unsigned int message_list_n;
		struct {
			uint32_t memory_index;
			QmiWmsMessageTagType message_tag;
		} *message_list;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_set_routes_request {
	struct {
		unsigned int transfer_status_report : 1;
	} set;
	struct {
		unsigned int route_list_n;
		struct {
			QmiWmsMessageType message_type;
			QmiWmsMessageClass message_class;
			QmiWmsStorageType storage;
			QmiWmsReceiptAction receipt_action;
		} *route_list;
		QmiWmsTransferIndication transfer_status_report;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_get_routes_response {
	struct {
		unsigned int transfer_status_report : 1;
	} set;
	struct {
		unsigned int route_list_n;
		struct {
			QmiWmsMessageType message_type;
			QmiWmsMessageClass message_class;
			QmiWmsStorageType storage;
			QmiWmsReceiptAction receipt_action;
		} *route_list;
		QmiWmsTransferIndication transfer_status_report;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_send_from_memory_storage_request {
	struct {
		unsigned int information : 1;
		unsigned int sms_on_ims : 1;
	} set;
	struct {
		struct {
			QmiWmsStorageType storage_type;
			uint32_t memory_index;
			QmiWmsMessageMode message_mode;
		} information;
		bool sms_on_ims;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

struct qmi_wms_send_from_memory_storage_response {
	struct {
		unsigned int message_id : 1;
		unsigned int cdma_cause_code : 1;
		unsigned int cdma_error_class : 1;
		unsigned int gsm_wcdma_cause_info : 1;
		unsigned int message_delivery_failure_type : 1;
	} set;
	struct {
		uint16_t message_id;
		QmiWmsCdmaCauseCode cdma_cause_code;
		QmiWmsCdmaErrorClass cdma_error_class;
		struct {
			QmiWmsGsmUmtsRpCause rp_cause;
			QmiWmsGsmUmtsTpCause tp_cause;
		} gsm_wcdma_cause_info;
		QmiWmsMessageDeliveryFailureType message_delivery_failure_type;
	} data;
	/** Private data: do not use! */
	struct {
		struct qmi_buf *qbuf;
	} priv;
};

int qmi_set_wms_reset_request(struct qmi_msg *msg);
int qmi_parse_wms_reset_response(struct qmi_msg *msg);
int qmi_set_wms_set_event_report_request(struct qmi_msg *msg, struct qmi_wms_set_event_report_request *req);
int qmi_parse_wms_set_event_report_response(struct qmi_msg *msg);
int qmi_set_wms_raw_send_request(struct qmi_msg *msg, struct qmi_wms_raw_send_request *req);
int qmi_parse_wms_raw_send_response(struct qmi_msg *msg, struct qmi_wms_raw_send_response *res);
void qmi_cleanup_wms_raw_send_response(struct qmi_wms_raw_send_response *res);

int qmi_set_wms_raw_write_request(struct qmi_msg *msg, struct qmi_wms_raw_write_request *req);
int qmi_parse_wms_raw_write_response(struct qmi_msg *msg, struct qmi_wms_raw_write_response *res);
void qmi_cleanup_wms_raw_write_response(struct qmi_wms_raw_write_response *res);

int qmi_set_wms_raw_read_request(struct qmi_msg *msg, struct qmi_wms_raw_read_request *req);
int qmi_parse_wms_raw_read_response(struct qmi_msg *msg, struct qmi_wms_raw_read_response *res);
void qmi_cleanup_wms_raw_read_response(struct qmi_wms_raw_read_response *res);

int qmi_set_wms_modify_tag_request(struct qmi_msg *msg, struct qmi_wms_modify_tag_request *req);
int qmi_parse_wms_modify_tag_response(struct qmi_msg *msg);
int qmi_set_wms_delete_request(struct qmi_msg *msg, struct qmi_wms_delete_request *req);
int qmi_parse_wms_delete_response(struct qmi_msg *msg);
int qmi_set_wms_get_message_protocol_request(struct qmi_msg *msg);
int qmi_parse_wms_get_message_protocol_response(struct qmi_msg *msg, struct qmi_wms_get_message_protocol_response *res);
void qmi_cleanup_wms_get_message_protocol_response(struct qmi_wms_get_message_protocol_response *res);

int qmi_set_wms_list_messages_request(struct qmi_msg *msg, struct qmi_wms_list_messages_request *req);
int qmi_parse_wms_list_messages_response(struct qmi_msg *msg, struct qmi_wms_list_messages_response *res);
void qmi_cleanup_wms_list_messages_response(struct qmi_wms_list_messages_response *res);

int qmi_set_wms_set_routes_request(struct qmi_msg *msg, struct qmi_wms_set_routes_request *req);
int qmi_parse_wms_set_routes_response(struct qmi_msg *msg);
int qmi_set_wms_get_routes_request(struct qmi_msg *msg);
int qmi_parse_wms_get_routes_response(struct qmi_msg *msg, struct qmi_wms_get_routes_response *res);
void qmi_cleanup_wms_get_routes_response(struct qmi_wms_get_routes_response *res);

int qmi_set_wms_send_from_memory_storage_request(struct qmi_msg *msg, struct qmi_wms_send_from_memory_storage_request *req);
int qmi_parse_wms_send_from_memory_storage_response(struct qmi_msg *msg, struct qmi_wms_send_from_memory_storage_response *res);
void qmi_cleanup_wms_send_from_memory_storage_response(struct qmi_wms_send_from_memory_storage_response *res);

