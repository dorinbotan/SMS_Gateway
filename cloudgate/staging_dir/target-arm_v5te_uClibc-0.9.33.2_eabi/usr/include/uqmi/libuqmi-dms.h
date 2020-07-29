#ifndef libuqmi_dms_h
#define libuqmi_dms_h

#include "qmi-message.h"

int uqmi_dms_get_iccid(struct qmi_dev *qmi, request_cb cb, void *arg);

#endif
