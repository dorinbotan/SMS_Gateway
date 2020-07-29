#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

// get size in bytes for all the logs
const char *get_log_path(void);
// gets the path where the log files are stored
long get_log_size(void);
int load_log_size(void);
int load_log_path(void);

#endif // CONFIG_H_INCLUDED
