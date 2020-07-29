#ifndef __DUST_SENSOR_ACCESS_H__
#define __DUST_SENSOR_ACCESS_H__
int dust_sensor_power(int fd, uint32_t power);
int dust_sensor_set_spi_mode(int fd);
int dust_sensor_measure(int fd,unsigned char *bytes,int len);

#endif // __DUST_SENSOR_ACCESS_H__
