#ifndef lizzylog_config_h
#define lizzylog_config_h

#include <time.h>
#include <lizzylog/log_types.h>

struct lizzylog_base_config {
	/** Log type */
	enum lizzylog_type type;
	/** Log type is enabled */
	int enabled;
	/** Interval in text format. This value is converted to the above interval attribute */
	char cinterval[32];
};

struct lizzylog_type_config {
	/** Base config */
	struct lizzylog_base_config base;
	/** Name of the type */
	char name[64];
	/** Interval on which a log for this type should be taken. Internal use only */
	time_t interval;
	/** Timestamp in the future. If exceeded, a write must happen */
	time_t write_timestamp;
};

struct lizzylog_config {
	/** Number of entries. Must match maximum enum lizzylog_type value */
	int entries;
	/** Information about each log type */
	struct lizzylog_type_config type_cfg[];
};

struct lizzylog_config* lizzylog_cfg_init();
struct lizzylog_type_config* lizzylog_cfg_get_type_config(struct lizzylog_config *cfg, enum lizzylog_type type);
/** Write only the write_timestamp to UCI */
int lizzylog_cfg_save_write_timestamp(struct lizzylog_type_config *cfg);
/** Write all except the write_timestamp to UCI */
int lizzylog_cfg_save_type_config(struct lizzylog_type_config *cfg);
time_t lizzylog_cfg_get_interval_from_string(const char *interval);
int lizzylog_base_config_equals(struct lizzylog_base_config *bc1, struct lizzylog_base_config *bc2);

#endif
