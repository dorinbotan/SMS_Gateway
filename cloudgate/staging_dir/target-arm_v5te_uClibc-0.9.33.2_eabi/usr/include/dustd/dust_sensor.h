#ifndef __DUST_SENSOR_H_
#define __DUST_SENSOR_H_

#include <stdint.h>

#define dustd_log(x, y, ...) log(LOG_DUSTD, x, y, ##__VA_ARGS__)

/** \enum dust_sensor_power_t Dust sensor's power settings */
typedef enum {
	/** When nor fan nor laser is powered */
	DUST_SENSOR_POWER_OFF = 0,
	/** Bit to set fan power */
	DUST_SENSOR_POWER_FAN_ON = 1,
	/** Bit to set laser power */
	DUST_SENSOR_POWER_LASER_ON =  2,
	/** Max value dust sensor's power */
	DUST_SENSOR_POWER_MAX = 3
} dust_sensor_power_t;

typedef enum {
	/** When valid data is available */
	DUST_SENSOR_VALID_DATA,
	/** When a read/write error occured */
	DUST_SENSOR_RW_ERROR,
	/** when the dust sensor disconnects */
	DUST_SENSOR_DISCONNECT,
	/** When the dust sensor connects */
	DUST_SENSOR_CONNECT,
	/** Dust sensor not available */
	DUST_SENSOR_NOT_AVAILABLE,
	/** Can't create thread */
	DUST_SENSOR_THREAD_CREATE_ERROR,
	/** Invalid parameter */
	DUST_SENSOR_INVALID_PARAMETER,
	/** Dust sensor is not ready */
	DUST_SENSOR_NOT_AWAKE,
	/** Error setting spi mode */
	DUST_SENSOR_SPI_ERROR
} dust_sensor_status_t;

typedef struct dust_sensor_data {
	/** 63 bytes raw logging data */
	unsigned char byte[63];
	/** 4 byte status number see dust_sensor_status_t for meaning */
	uint32_t status;
} dust_sensor_data_t;


int dust_sensor_available(void);
int dust_sensor_open(char *file_name);
void dust_sensor_close(void);
int dust_sensor_start(uint32_t reporting_interval);
int dust_sensor_stop(void);
int dust_sensor_set_power(uint32_t power);
void dust_sensor_notify(dust_sensor_data_t *dust_sensor_data);
void dust_sensor_notify_synced(void *arg);
#endif // __DUST_SENSOR_H_
