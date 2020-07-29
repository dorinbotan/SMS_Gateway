#ifndef libuqmi_nas_h
#define libuqmi_nas_h

#include "qmi-message.h"

int uqmi_nas_network_scan(struct qmi_dev *qmi, request_cb cb, void *arg);
int uqmi_nas_set_rat_mode_preference(struct qmi_dev *qmi, request_cb cb, void *arg, QmiNasRatModePreference rat_preference);
int uqmi_nas_get_system_selection_preference(struct qmi_dev *qmi, request_cb cb, void *arg);
int uqmi_nas_get_signal_info(struct qmi_dev *qmi, request_cb cb, void *arg);
#endif
