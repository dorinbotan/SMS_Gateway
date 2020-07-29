
#ifndef _SI3452_H_
#define _SI3452_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <termios.h>

#define READ_COMMAND_CODE		1
#define WRITE_COMMAND_CODE		0

//register addresses
#define INTERRUPT_REG_ADDR			0x00
#define INTERRUPT_MASK_ADDR			0x01
#define PORT1_EVENTS_ADDR			0x02
#define PORT2_EVENTS_ADDR			0x03
#define PORT3_EVENTS_ADDR			0x04
#define PORT4_EVENTS_ADDR			0x05
#define PORT1_STATUS_ADDR			0x06
#define PORT2_STATUS_ADDR			0x07
#define PORT3_STATUS_ADDR			0x08
#define PORT4_STATUS_ADDR			0x09
#define PORT1_CONFIG_ADDR			0x0A
#define PORT2_CONFIG_ADDR			0x0B
#define PORT3_CONFIG_ADDR			0x0C
#define PORT4_CONFIG_ADDR			0x0D
#define PORT1_ICUT_CONFIG_ADDR		0x0E
#define PORT2_ICUT_CONFIG_ADDR		0x0F
#define PORT3_ICUT_CONFIG_ADDR		0x10
#define PORT4_ICUT_CONFIG_ADDR		0x11
#define COMMAND_REG_ADDR			0x12
#define VEE_MSB_ADDR				0x13
#define VEE_LSB_ADDR				0x14
#define PORT1_CURRENT_MSB_ADDR		0x15
#define PORT1_CURRENT_LSB_ADDR		0x16
#define PORT2_CURRENT_MSB_ADDR		0x17
#define PORT2_CURRENT_LSB_ADDR		0x18
#define PORT3_CURRENT_MSB_ADDR		0x19
#define PORT3_CURRENT_LSB_ADDR		0x1A
#define PORT4_CURRENT_MSB_ADDR		0x1B
#define PORT4_CURRENT_LSB_ADDR		0x1C
#define DEVICE_STATUS_ADDR			0x1D
#define HW_REVISION_ADDR			0x60
#define FW_REVISION_ADDR			0x61

#define FW_REVISION_ADDR_LENGTH		0x03


//command codes
#define POWER_ON_PORT_CMD					0x04
#define POWER_OFF_PORT_CMD					0x08
#define RESET_PORT_CMD						0x0C
#define TOGGLE_DET_BACKOFF_TIMING_CMD		0x10
#define RESET_CHIP_CMD						0x14
#define READ_VEE_CMD						0x18
#define READ_PORT_CURRENT_CMD				0x1C
#define SHUT_DOWN_LOW_PRIORITY_PORTS_CMD	0x20

//detect status codes
#define DETECT_UNKNOWN						0x00
#define DETECT_SHORT						0x01
#define DETECT_RESERVED1					0x02
#define DETECT_R_LOW						0x03
#define DETECT_GOOD							0x04
#define DETECT_R_HIGH						0x05
#define DETECT_R_OPEN						0x06
#define DETECT_RESERVED2					0x07

//class codes
#define CLASS_UNKNOWN						0x00
#define CLASS_CLASS1						0x01
#define CLASS_CLASS2						0x02
#define CLASS_CLASS3						0x03
#define CLASS_CLASS4						0x04
#define CLASS_PROBES_NOT_EQUAL				0x05
#define CLASS_CLASS0						0x06
#define CLASS_OVERLOAD						0x07

//port mode codes
#define PORT_SHUTDOWN						0x00
#define PORT_MANUAL							0x01
#define PORT_SEMI_AUTO						0x02
#define PORT_AUTO							0x03

//bit masks
#define PWR_GOOD_STATUS_MASK				0x80
#define PWR_ENABLE_STATUS_MASK				0x40
#define CLASS_STATUS_MASK					0x38
#define DETECT_STATUS_MASK					0x07
#define PORT_MODE_CONFIG_MASK				0x03
#define DISC_EN_CONFIG_MASK					0x04
#define POE_PLUS_CONFIG_MASK				0x08
#define PORT_PRIORITY_CONFIG_MASK			0x10
#define DEVICE_STATUS_OVERTEMP_MASK			0x40
#define DEVICE_STATUS_VEE_UVLO_MASK			0x20
#define DEVICE_STATUS_ALTB_TIMING_MASK		0x00

//interrupt bit masks
#define INT_ENABLE_ALL						0xFF
#define INT_DEVICE_STATUS_MASK				0x80
#define INT_TSTART_MASK						0x40
#define INT_TICUT_MASK						0x20
#define INT_RGOOD_CLS_MASK					0x10
#define INT_DET_COMPL_MASK					0x08
#define INT_DISCONNECT_MASK					0x04
#define INT_PWRGOOD_MASK					0x02
#define INT_PWREN_MASK						0x01
#define INT_DISABLE_ALL						0x00

//interrupt status masks
#define INT_STATUS_OVERTEMP_CHANGE			0x40
#define INT_STATUS_UVLO_CHANGE				0x20
#define INT_STATUS_PORT4_EVENT				0x08
#define INT_STATUS_PORT3_EVENT				0x04
#define INT_STATUS_PORT2_EVENT				0x02
#define INT_STATUS_PORT1_EVENT				0x01

//bit shift numbers
#define PWR_GOOD_STATUS_BIT_SHIFT			0x07
#define PWR_ENABLE_STATUS_BIT_SHIFT			0x06
#define CLASS_STATUS_BIT_SHIFT				0x03
#define DETECT_STATUS_BIT_SHIFT				0x00
#define PORT_MODE_CONFIG_BIT_SHIFT			0x00
#define DISC_EN_CONFIG_BIT_SHIFT			0x02
#define POE_PLUS_CONFIG_BIT_SHIFT			0x03
#define PORT_PRIORITY_CONFIG_BIT_SHIFT		0x04
#define DEVICE_STATUS_OVERTEMP_BIT_SHIFT	0x06
#define DEVICE_STATUS_VEE_UVLO_BIT_SHIFT	0x05
#define DEVICE_STATUS_ALTB_TIMING_BIT_SHIFT	0x00

//special codes
#define INTERRUPT_OCCURED					0x35

typedef void (*si3452_interrupt_cb_t)(void *arg);

int send_data (int fd, char *data, unsigned int size, struct timeval *tv);
int read_data (int fd, char *data, unsigned int size, struct timeval *tv);

int write_register (int fd, unsigned char chRegisterAddress, unsigned char chRegisterSize, unsigned char *data);
int read_register (int fd, unsigned char chRegisterAddress, unsigned char chRegisterSize, unsigned char *data);
int power_on_port (int fd, unsigned char chPortNumber);
int power_off_port (int fd, unsigned char chPortNumber);
int reset_port (int fd, unsigned char chPortNumber);
int reset_chip (int fd);
int toggle_detection_backoff_timing (int fd);
int shut_down_low_priority_ports (int fd);
int read_vee (int fd, unsigned short *nVee);
int read_port_current (int fd, unsigned char chPortNumber, unsigned short *nCurrent);
int read_port_power_good_status (int fd, unsigned char chPortNumber, unsigned char *chPowerGoodStatus);
int read_port_power_enable_status (int fd, unsigned char chPortNumber, unsigned char *chPowerEnableStatus);
int read_port_class_status (int fd, unsigned char chPortNumber, unsigned char *chClassStatus);
int read_port_detect_status (int fd, unsigned char chPortNumber, unsigned char *chDetectStatus);
int read_port_priority_config (int fd, unsigned char chPortNumber, unsigned char *chPortPriority);
int read_port_poe_plus_config (int fd, unsigned char chPortNumber, unsigned char *chPortPoePlus);
int read_port_disconnect_enable_config (int fd, unsigned char chPortNumber, unsigned char *chDisconnectEnable);
int read_port_mode_config (int fd, unsigned char chPortNumber, unsigned char *chPortMode);
int write_port_priority_config (int fd, unsigned char chPortNumber, unsigned char chPortPriority);
int write_port_poe_plus_config (int fd, unsigned char chPortNumber, unsigned char chPortPoePlus);
int write_port_disconnect_enable_config (int fd, unsigned char chPortNumber, unsigned char chDisconnectEnable);
int write_port_mode_config (int fd, unsigned char chPortNumber, unsigned char chPortMode);
int read_port_i_cut_config (int fd, unsigned char chPortNumber, unsigned char *chICut);
int write_port_i_cut_config (int fd, unsigned char chPortNumber, unsigned char chICut);
int read_interrupt_mask (int fd, unsigned char *chInterruptMask);
int write_interrupt_mask_config (int fd, unsigned char chInterruptMask);
int read_interrupt_status (int fd, unsigned char *chInterruptStatus);
int read_port_events (int fd, int chPortId, unsigned char *chPortEvents);
int read_port_status (int fd, int chPortId, unsigned char *chPortStatus);
int read_device_status (int fd, unsigned char *chDeviceStatus);
int read_hw_revision (int fd, unsigned char *chHwRevision);
int read_fw_revision (int fd, unsigned char *chFwRevision);
int read_device_overtemp_status (int fd, unsigned char *chOvertempStatus);
int read_device_vee_uvlo_status (int fd, unsigned char *chVeeUvloStatus);
int read_device_alternative_b_timing_status (int fd, unsigned char *chAlternativeBTimingStatus);
int si3452_register_interrupt_callback(si3452_interrupt_cb_t cb);

#endif



