#ifndef LEDS_CG_INTERNAL_H
#define LEDS_CG_INTERNAL_H

#include <linux/leds.h>

enum led_ids {
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
	LED6,
	LED7,
	LED_NUM,
	ALL_LEDS,
};

enum led_colors {
	GREEN,
	RED,
	ORANGE,
	ALL_COLORS,
};

/*
 * State '0' : 'off'
 * State 'x' : 'on'.
 */
typedef struct {
	unsigned int r;
	unsigned int g;
	unsigned int b;//extra red
} led_state;

typedef struct {
	led_state		led[7];

	/*
	 * Making led_classdev as array is not recommended, because array
	 * members prevent using 'container_of' macro. So repetitive works
	 * are needed.
	 */
	struct led_classdev		cdev_led1r;
	struct led_classdev		cdev_led1g;
	struct led_classdev		cdev_led1orange;

	struct led_classdev		cdev_led2r;
	struct led_classdev		cdev_led2g;
	struct led_classdev		cdev_led2orange;

	struct led_classdev		cdev_led3r;
	struct led_classdev		cdev_led3g;
	struct led_classdev		cdev_led3orange;

	struct led_classdev		cdev_led4r;
	struct led_classdev		cdev_led4g;
	struct led_classdev		cdev_led4orange;

	struct led_classdev		cdev_led5r;
	struct led_classdev		cdev_led5g;
	struct led_classdev		cdev_led5orange;

	struct led_classdev		cdev_led6r;
	struct led_classdev		cdev_led6g;
	struct led_classdev		cdev_led6orange;

	struct led_classdev		cdev_led7r;
	struct led_classdev		cdev_led7g;
	struct led_classdev		cdev_led7orange;

	struct led_classdev		cdev_7leds3colors;
} leds_cg_led;

void leds_cg_write_shift_register(leds_cg_led *led);
void led_exit(void);
#endif
