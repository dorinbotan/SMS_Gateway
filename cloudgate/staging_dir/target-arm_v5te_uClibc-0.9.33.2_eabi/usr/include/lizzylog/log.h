#ifndef lizzylog_h
#define lizzylog_h

#include <lizzylog/log_types.h>

int lizzylog_init(const char *filename);
int lizzylog_deinit();

/**
 * Write one or multiples entries of the same type. In case of string, you have to pass a double pointer.
 * The lizzylog_write_entry calls lizzylog_write_multiple_entries with num_entries set to 1.
 */
int lizzylog_write_entry(enum lizzylog_type t, enum lizzylog_entry_type et, void *val);
int lizzylog_write_multi_entries(enum lizzylog_type t, enum lizzylog_entry_type et, int num_entries, void *val);
void lizzylog_set_max_entries(unsigned int max_entries);
unsigned int lizzylog_get_max_entries();
unsigned int lizzylog_get_current_entries();

#endif
