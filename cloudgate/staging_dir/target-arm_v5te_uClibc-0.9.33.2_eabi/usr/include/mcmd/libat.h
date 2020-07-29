#ifndef LIBAT_H
#define LIBAT_H
#include <log/log.h>
#include <stdint.h>

/**Log function**/
#define libat_log(x, y, ...) log(LOG_LIBAT, x, y, ##__VA_ARGS__) 

typedef struct {
	char *command;
	char *arg;
	char *modem;
} at_command_t;

typedef void (*at_user_data_cb)(int status, void *data, uint32_t length, void *ctx);
int32_t at_read(int fd, void *arg);
int32_t at_write(const char *command, const char *expected_result, at_user_data_cb cb);
int at_init(char *dev_name, const int nonblocking);
int at_deinit();
at_command_t *at_command_init(char *command, char *arg, char *modem);
int at_command_deinit(at_command_t *at_cmd);

#endif
