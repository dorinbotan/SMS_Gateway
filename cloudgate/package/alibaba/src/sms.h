#ifndef __SMS_H__
#define __SMS_H__

typedef void (*sms_receive_callback_t)(const char *sender, const char *text);

int sms_init();
int sms_send(char *number, char *text);
int sms_onreceive(sms_receive_callback_t callback);
void sms_deinit();

#endif /*__SMS_H_*/
