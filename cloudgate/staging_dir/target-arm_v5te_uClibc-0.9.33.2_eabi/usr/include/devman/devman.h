

#ifndef DEVMAN_H
#define DEVMAN_H

typedef enum {
	ARRIVED,
	LEFT,
	CLAIMED,
	RELEASED
} device_event_t;

static inline char* device_event_to_str(device_event_t event) {
	switch(event) {
		case ARRIVED:  return "ARRIVED";  break;
		case LEFT:     return "LEFT";     break;
		case CLAIMED:  return "CLAIMED";  break;
		case RELEASED: return "RELEASED"; break;
	}
	return NULL;
}

typedef enum {
	NOT_PRESENT,
	PRESENT,
	PRESENCE_UNKNOWN
} presence_t;

static inline char* presence_to_str(presence_t presence) {
	switch(presence) {
		case NOT_PRESENT:      return "NOT_PRESENT";      break;
		case PRESENT:          return "PRESENT";          break;
		case PRESENCE_UNKNOWN: return "PRESENCE_UNKNOWN"; break;
	}
	return NULL;
}

typedef enum {
	DEV_CAUSE_NONE,
	DEV_CAUSE_PM,
	DEV_CAUSE_COLD,
	DEV_CAUSE_HOT,
	DEV_CAUSE_UPGRADE,
	DEV_CAUSE_CONFIG,
	DEV_CAUSE_REBOOT,
	DEV_CAUSE_OTA
} device_cause_t;

static inline char* device_cause_to_str(device_cause_t device_cause) {
	switch(device_cause) {
		case DEV_CAUSE_NONE:     return "DEV_CAUSE_NONE";     break;
		case DEV_CAUSE_PM:       return "DEV_CAUSE_PM";       break;
		case DEV_CAUSE_COLD:     return "DEV_CAUSE_COLD";     break;
		case DEV_CAUSE_HOT:      return "DEV_CAUSE_HOT";      break;
		case DEV_CAUSE_UPGRADE:  return "DEV_CAUSE_UPGRADE";  break;
		case DEV_CAUSE_CONFIG:   return "DEV_CAUSE_CONFIG";   break;
		case DEV_CAUSE_REBOOT:   return "DEV_CAUSE_REBOOT";   break;
		case DEV_CAUSE_OTA:      return "DEV_CAUSE_OTA";      break;
	}
	return NULL;
}

typedef enum {
	LOC_UNKNOWN,
	LOC_SLOT1, // front side with leds
	LOC_SLOT2, // back side with power connector
	LOC_MAIN,
	LOC_MAX
} location_t;

typedef enum {
	DEVTYPE_NETWORK,
	DEVTYPE_SERIAL,
	DEVTYPE_DAC,
	DEVTYPE_GPIO,
	DEVTYPE_ADC,
	DEVTYPE_INVALID
} dev_type_t;

static inline char* dev_type_to_str(dev_type_t dev_type) {
	switch(dev_type) {
		case DEVTYPE_NETWORK:  return "DEVTYPE_NETWORK";      break;
		case DEVTYPE_SERIAL:   return "DEVTYPE_SERIAL";       break;
		case DEVTYPE_DAC:      return "DEVTYPE_DAC";          break;
		case DEVTYPE_GPIO:     return "DEVTYPE_GPIO";         break;
		case DEVTYPE_ADC:      return "DEVTYPE_ADC";          break;
		case DEVTYPE_INVALID:  return "DEVTYPE_INVALID";      break;
	}
	return NULL;
}

typedef enum {
	CLAIM_DENIED,
	CLAIM_FRESH,
	CLAIM_YOURS,
	CLAIM_GRANTED
} claim_type_t;

//input type for the the update_device rpc
/* TODO currently only when a device will be removed an update is send,
 * maybe one day we might need this in other cases too, add some sort of event-that-will-happen type
 */
typedef struct {
	device_cause_t cause;
	char name[];
} device_update_t;

#endif
