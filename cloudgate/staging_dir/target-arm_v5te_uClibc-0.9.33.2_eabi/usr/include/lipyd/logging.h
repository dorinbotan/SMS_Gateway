#ifndef LOGGING_H
#define LOGGING_H

typedef struct {
	int enabled;
	long int max_size;
	char levels[128];
} log_settings_t;



void init_logging_settings_rpcs(int* fd);

#endif
