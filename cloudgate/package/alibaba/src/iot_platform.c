#include "iot_platform.h"

#include "iot_import.h"
#include "iot_export.h"
#include "CGHelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE_TRACE(fmt, ...)  \
    do { \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__); \
        HAL_Printf("%s", "\r\n"); \
    } while(0)

int rc = 0;
void *pclient;
iotx_conn_info_pt pconn_info;
iotx_mqtt_param_t mqtt_params;
iotx_mqtt_topic_info_t topic_msg;
char msg_pub[MSG_MAX_LEN]; /* 918 - maximum SMS length */
char *msg_buf = NULL, *msg_readbuf = NULL;
iot_platform_receive_callback_t mqtt_receive_callback = NULL;

void event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    uintptr_t packet_id = (uintptr_t)msg->msg;
    iotx_mqtt_topic_info_pt topic_info = (iotx_mqtt_topic_info_pt)msg->msg;

    switch (msg->event_type) {
        case IOTX_MQTT_EVENT_UNDEF:
            EXAMPLE_TRACE("undefined event occur.");
            break;

        case IOTX_MQTT_EVENT_DISCONNECT:
            EXAMPLE_TRACE("MQTT disconnect.");
            break;

        case IOTX_MQTT_EVENT_RECONNECT:
            EXAMPLE_TRACE("MQTT reconnect.");
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
            EXAMPLE_TRACE("subscribe success, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
            EXAMPLE_TRACE("subscribe wait ack timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
            EXAMPLE_TRACE("subscribe nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
            EXAMPLE_TRACE("unsubscribe success, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
            EXAMPLE_TRACE("unsubscribe timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
            EXAMPLE_TRACE("unsubscribe nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
            EXAMPLE_TRACE("publish success, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
            EXAMPLE_TRACE("publish timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_NACK:
            EXAMPLE_TRACE("publish nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_RECVEIVED:
            EXAMPLE_TRACE("topic message arrived but without any related handle: topic=%.*s, topic_msg=%.*s",
                          topic_info->topic_len,
                          topic_info->ptopic,
                          topic_info->payload_len,
                          topic_info->payload);
            break;

        case IOTX_MQTT_EVENT_BUFFER_OVERFLOW:
            EXAMPLE_TRACE("buffer overflow, %s", msg->msg);
            break;

        default:
            EXAMPLE_TRACE("Should NOT arrive here.");
            break;
    }
}

int iot_platform_init()
{
    #ifdef DEBUG
    IOT_OpenLog("mqtt");
    IOT_SetLogLevel(IOT_LOG_DEBUG);
    #endif /* DEBUG */

    if (NULL == (msg_buf = (char *)HAL_Malloc(MQTT_MSGLEN))) 
    {
        EXAMPLE_TRACE("not enough memory");
        return 0;
    }

    if (NULL == (msg_readbuf = (char *)HAL_Malloc(MQTT_MSGLEN))) 
    {
        EXAMPLE_TRACE("not enough memory");
        return 0;
    }

    /* Device AUTH */
    if (0 != IOT_SetupConnInfo(PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, (void **)&pconn_info)) 
    {
        EXAMPLE_TRACE("AUTH request failed!");
        return 0;
    }

    /* Initialize MQTT parameter */
    memset(&mqtt_params, 0x00, sizeof(mqtt_params));

    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----%d\n", pconn_info->port);
    printf("-----%s\n", pconn_info->host_name);
    printf("-----%s\n", pconn_info->client_id);
    printf("-----%s\n", pconn_info->username);
    printf("-----%s\n", pconn_info->password);
    printf("-----%s\n", pconn_info->pub_key);
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");

    mqtt_params.port = pconn_info->port;
    mqtt_params.host = pconn_info->host_name;
    mqtt_params.client_id = pconn_info->client_id;
    mqtt_params.username = pconn_info->username;
    mqtt_params.password = pconn_info->password;
    mqtt_params.pub_key = pconn_info->pub_key;

    mqtt_params.request_timeout_ms = 2000;
    mqtt_params.clean_session = 0;
    mqtt_params.keepalive_interval_ms = 60000;
    mqtt_params.pread_buf = msg_readbuf;
    mqtt_params.read_buf_size = MQTT_MSGLEN;
    mqtt_params.pwrite_buf = msg_buf;
    mqtt_params.write_buf_size = MQTT_MSGLEN;

    mqtt_params.handle_event.h_fp = event_handle;
    mqtt_params.handle_event.pcontext = NULL;

    /* Construct a MQTT client with specify parameter */
    pclient = IOT_MQTT_Construct(&mqtt_params);
    if (NULL == pclient) 
    {
        EXAMPLE_TRACE("MQTT construct failed");
        return 0;
    }

    return 1;
}

int iot_platform_send(const char *topic, const char *text)
{
    #ifdef DEBUG
    printf("MQTT pub: %s", text);
    #endif /* DEBUG */

    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0;
    topic_msg.dup = 0;
    topic_msg.payload = (char *)msg_pub;
    topic_msg.payload_len = snprintf(msg_pub, sizeof(msg_pub), text);

    rc = IOT_MQTT_Publish(pclient, topic, &topic_msg);
    if (rc < 0) 
    {
        EXAMPLE_TRACE("error occur when publish");
        return 0;
    }

    return 1;
}

void _iot_platform_receive_callback(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    if (!mqtt_receive_callback)
        return;

    iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt) msg->msg;

    #ifdef DEBUG
    /* print topic name and topic message */
    EXAMPLE_TRACE("-----------------------");
    EXAMPLE_TRACE("packetId: %d", ptopic_info->packet_id);
    EXAMPLE_TRACE("Topic: '%.*s' (Length: %d)",
                    ptopic_info->topic_len,
                    ptopic_info->ptopic,
                    ptopic_info->topic_len);
    EXAMPLE_TRACE("Payload: '%.*s' (Length: %d)",
                    ptopic_info->payload_len,
                    ptopic_info->payload,
                    ptopic_info->payload_len);
    EXAMPLE_TRACE("----------------------");
    #endif /* DEBUG */

    char msg_to_send[ptopic_info->payload_len + 1];
    snprintf(msg_to_send, sizeof(msg_to_send), "%s", ptopic_info->payload);

    mqtt_receive_callback(msg_to_send);
}

/* TODO: (DOBO) tie to a specific topic */
/* NOTE: (DOBO) make sure to properly unsubscribe in mqtt_deinit() */
int iot_platform_onreceive(iot_platform_receive_callback_t callback)
{
    mqtt_receive_callback = callback;

    rc = IOT_MQTT_Subscribe(pclient, TOPIC_OUT, IOTX_MQTT_QOS1, _iot_platform_receive_callback, NULL);
    if (rc < 0)
    {
        IOT_MQTT_Destroy(&pclient);
        EXAMPLE_TRACE("IOT_MQTT_Subscribe() failed, rc = %d", rc);
        return 0;
    }

    IOT_MQTT_Yield(pclient, 200);

    return 1;
}

void iot_platform_listen()
{
    while(1)
    {
        /* Handle the MQTT packet received from TCP or SSL connection */
        IOT_MQTT_Yield(pclient, 200);
        HAL_SleepMs(1000);
    }
}

void iot_platform_deinit() 
{
    IOT_MQTT_Yield(pclient, 200);
    IOT_MQTT_Unsubscribe(pclient, TOPIC_OUT);

    IOT_MQTT_Yield(pclient, 200);
    IOT_MQTT_Destroy(&pclient);

    if (NULL != msg_buf)
        HAL_Free(msg_buf);

    if (NULL != msg_readbuf)
        HAL_Free(msg_readbuf);

    #ifdef DEBUG
    IOT_DumpMemoryStats(IOT_LOG_DEBUG);
    IOT_CloseLog();
    #endif /* DEBUG */
}
