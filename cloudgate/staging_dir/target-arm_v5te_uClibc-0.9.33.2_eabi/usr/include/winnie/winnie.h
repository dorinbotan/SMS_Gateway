#ifndef WINNIE_H
#define WINNIE_H

#include <stdint.h>

#define POE_MAX_POWER 60
#define POE_NOTIFICATION_LIST "poe_notification_list"

typedef enum {
	POE_0W,
	POE_15W,
	POE_30W,
	POE_UNKNOWN
}poe_value_t;

typedef struct {
	uint16_t max;
	int enabled;
	poe_value_t poe_values[4];
}poe_config_t;

typedef struct {
	uint8_t capable;
	uint8_t powered;
}poe_status_t;

typedef enum {
	POE_PORT_POWERED,
	POE_PORT_UNPOWERED, // Because it's disabled or because there is no device connected
	POE_PORT_OVERPOWERED,
	POE_PORT_WRONG_CLASS,
	POE_PORT_RETRYING
}poe_port_state_t;

typedef struct {
	int power;
	poe_port_state_t status;
	int power_failed_counter;
	poe_value_t value;
}poe_port_status_t;

typedef struct {
	int voltage;
	int peak_power;
	int overtemp_flag;
	int voltage_out_of_range_flag;
	poe_port_status_t port[4];
}poe_global_status_t;

static inline poe_value_t number_to_poe_enum(int number)
{
	switch(number)
	{
		default:
			return POE_UNKNOWN;
		case 0:
			return POE_0W;
		case 15:
			return POE_15W;
		case 30:
			return POE_30W;
	}
}

static inline int poe_enum_to_number(poe_value_t value)
{
	switch(value)
	{
		case POE_0W:
			return 0;
		case POE_15W:
			return 15;
		case POE_30W:
			return 30;
		default:
			return -1;
	}
}

#endif
