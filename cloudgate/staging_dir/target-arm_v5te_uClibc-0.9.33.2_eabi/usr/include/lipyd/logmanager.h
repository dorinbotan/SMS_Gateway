#ifndef LOGMANAGER_H_INCLUDED
#define LOGMANAGER_H_INCLUDED

void logmanager(void);
void check_log_numbers(void);
void logmanager_stop_sleep(void);
void logmanager_start_sleep(int timeout_ms);
int logmanager_flush_log(void);

#endif // LOGMANAGER_H_INCLUDED
