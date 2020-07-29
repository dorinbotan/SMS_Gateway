#ifndef libuqmi_buffer_h
#define libuqmi_buffer_h

#include "libuqmi.h"
#include "mbim.h"

struct qmi_msgbuf {
	struct mbim_command_message mbim;
	union {
		char buf[2048];
		struct qmi_msg msg;
	} u;
} __packed;

struct qmi_msg* qmi_new_msg_buf();
char* qmi_buffer_get_buf(struct qmi_msgbuf *msg);
struct qmi_msg* qmi_buffer_get_qmi_msg(struct qmi_msgbuf *msg);

#endif
