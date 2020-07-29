#ifndef __IOT_PLATFORM_H__
#define __IOT_PLATFORM_H__

#include "config.h"
#include "iot_import.h"

typedef void (*iot_platform_receive_callback_t)(char *content);

#define TOPIC_IN           "/"PRODUCT_KEY"/"DEVICE_NAME"/user/sms/in"
#define TOPIC_OUT          "/"PRODUCT_KEY"/"DEVICE_NAME"/user/sms/out"

#define MQTT_MSGLEN        (4096) /* (1024) */
#define MSG_MAX_LEN        (8192) /* (2024) */

int iot_platform_init();
int iot_platform_send(const char *topic, const char *text);
int iot_platform_onreceive(iot_platform_receive_callback_t callback);
void iot_platform_listen();
void iot_platform_deinit();

#endif /*__IOT_PLATFORM_H__ */
