#ifndef libuqmi_wms_h
#define libuqmi_wms_h

#include "qmi-message.h"

struct uqmi_wms_sms_list_request {
	QmiWmsStorageType storage_type;
	QmiWmsMessageTagType message_tag;
	QmiWmsMessageMode message_mode;
};

struct uqmi_wms_sms_request {
	QmiWmsStorageType storage_type;
	QmiWmsMessageMode message_mode;
	uint32_t memory_index;
};

struct uqmi_wms_sms_send_request {
	QmiWmsMessageFormat format;
	unsigned int data_size;
	uint8_t *data;
};

int uqmi_wms_sms_list(struct qmi_dev *qmi, request_cb cb, void *arg, struct uqmi_wms_sms_list_request *sms_req);
int uqmi_wms_sms_read(struct qmi_dev *qmi, request_cb cb, void *arg, struct uqmi_wms_sms_request *sms_req);
int uqmi_wms_sms_delete(struct qmi_dev *qmi, request_cb cb, void *arg, struct uqmi_wms_sms_request *sms_req);
int uqmi_wms_sms_send(struct qmi_dev *qmi, request_cb cb, void *arg, struct uqmi_wms_sms_send_request *sms_req);

#endif
