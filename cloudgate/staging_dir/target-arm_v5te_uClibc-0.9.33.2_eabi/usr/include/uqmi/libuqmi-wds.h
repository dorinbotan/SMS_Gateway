#ifndef libuqmi_wds_h
#define libuqmi_wds_h

#include "qmi-message.h"

int uqmi_wds_get_current_settings(struct qmi_dev *qmi, request_cb cb, void *arg);
int uqmi_wds_get_packet_service_status(struct qmi_dev *qmi, request_cb cb, void *arg);
int uqmi_wds_start_network_request(struct qmi_dev *qmi, request_cb cb, void *arg);
int uqmi_wds_stop_network_request(struct qmi_dev *qmi, request_cb cb, void *arg);
#endif
