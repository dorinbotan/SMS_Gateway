#ifndef logger_types_h
#define logger_types_h

enum lizzylog_type {
	LOG_TYPE_NIC_RX_TX_BYTES, //0
	LOG_TYPE_ETHSWITCH_RX_TX_BYTES,
	LOG_TYPE_GOBI_RX_TX_BYTES,
	LOG_TYPE_PXS8_RX_TX_BYTES,
	LOG_TYPE_TELIT_RX_TX_BYTES,
	LOG_TYPE_UAP0_RX_TX_BYTES, //5
	LOG_TYPE_UAP1_RX_TX_BYTES,
	LOG_TYPE_MLAN0_RX_TX_BYTES,
	LOG_TYPE_HPEX_RX_TX_BYTES,
	LOG_TYPE_LAST_GPS_FIX,
	LOG_TYPE_COMBINED_SIGNAL_STRENGTH, //10
	LOG_TYPE_SYSTEM_UPTIME,
	LOG_TYPE_INTERFACE_CHECKIN,
	LOG_TYPE_WIFI_CLIENT_COUNT,
	LOG_TYPE_MAX
};

enum lizzylog_entry_type {
	LOG_ENTRY_TYPE_INT,
	LOG_ENTRY_TYPE_LONG_LONG,
	LOG_ENTRY_TYPE_U_LONG_LONG,
	LOG_ENTRY_TYPE_LONG,
	LOG_ENTRY_TYPE_U_LONG,
	LOG_ENTRY_TYPE_STRING,
	LOG_ENTRY_TYPE_CHAR,
	LOG_ENTRY_TYPE_MAX
};

static inline int lizzylog_is_valid_type(enum lizzylog_type type)
{
	return (type >= 0 && type < LOG_TYPE_MAX);
}

static inline int lizzylog_is_valid_entry_type(enum lizzylog_entry_type type)
{
	return (type >= 0 && type < LOG_ENTRY_TYPE_MAX);
}

#endif
