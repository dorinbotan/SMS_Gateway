#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcg/cg_general.h>
#include "json.h"

#include "config.h"
#include "CGHelper.h"
#include "sms.h"
#include "iot_platform.h"

/* {"Receiver":"+4529176086", "Message":"Hello from IoT Platform debug console"} */
void iot_platform_received_callback(char *content)
{
    #ifdef DEBUG
    printf("Transmit: %s\n", content);
    #endif /* DEBUG */

    char *receiver = (char *)getKeyFromValue(content, "Receiver");
    char *message = (char *)getKeyFromValue(content, "Message");

    if (receiver)
    {
        char receiver_edited[strlen(receiver)];
        for (int i = 1; i < strlen(receiver) - 1; i++)
            receiver_edited[i - 1] = receiver[i];

        char message_edited[strlen(message)];
        for (int i = 1; i < strlen(message) - 1; i++)
            message_edited[i - 1] = message[i];

        sms_send(receiver_edited, message_edited);
    }
    else 
    {
        sms_send(DEBUG_PHONE_NUMBER, message);
    }
}

void sms_received_callback(const char *sender, const char *content)
{
    #ifdef DEBUG
    printf("Sender: %s\n", sender);
    printf("Message: %s\n", content);
    #endif /* DEBUG */

    char msg[MSG_MAX_LEN];
    snprintf(msg, MSG_MAX_LEN, "{\"Sender\":\"%s\",\"Message\":\"%s\"}", sender, content);

    iot_platform_send(TOPIC_IN, msg);
}

void sms_greet()
{
    char buffer[64] = { 0 };
    snprintf(buffer, 64, "%u: CloudGate %s started", getUnixTime(), getSerialNumber());

    sms_send(DEBUG_PHONE_NUMBER, buffer);
}

int main(int argc, char **argv)
{
    cg_init("SMS proxy");
    sms_init();
    iot_platform_init();

    sms_onreceive(sms_received_callback);
    iot_platform_onreceive(iot_platform_received_callback);

    sms_greet();

    iot_platform_listen();

    iot_platform_deinit();
    sms_deinit();
    cg_deinit();

    return 1;
}
