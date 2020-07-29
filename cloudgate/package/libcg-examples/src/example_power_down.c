/* EXAMPLE:
 *  cg_system_power_down
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <libcg/cg_general.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

// Example_power_down demostrates shutdown of the Cloudgate when the engine is switched off.
// The example waits until the ignition line is high (indicates engine is turning). 
// When the application detects a high ignition sense level, it waits until ignition line 
// changes back to low level (engine switched off) to shutdown the Cloudgate.
// If the bit CG_SYSTEM_ARM_IGNITION_SENSE of the system_boot_condition is set, 
// the system will boot when ignition line changes to high level after a system shutdown.

/****************************************************************
 * Constants
 ****************************************************************/
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64
#define IGNITION_SENSE_PIN 263

/****************************************************************
 * gpio_export : registers a gpio
 * parameter gpio [in]  : gpio number
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_unexport : unregisters a gpio
 * parameter gpio [in]  : gpio number
 ****************************************************************/
int gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];
	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}


/****************************************************************
 * gpio_get_value
 * parameter gpio [in]  : gpio number
 * parameter value [out]: level of the gpio
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
	int fd, len;
	char buf[MAX_BUF];
	char ch;

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("gpio/get-value");
		return fd;
	}
 	lseek(fd, 0, SEEK_SET);
	read(fd, &ch, 1);

	if (ch != '0') {
		*value = 1;
	} else {
		*value = 0;
	}

	close(fd);
	return 0;
}


/****************************************************************
 * gpio_set_edge
 * parameter gpio [in]: gpio number
 * parameter egde [in]: interrupt type ( rising edge, falling edge or both edges)
 ****************************************************************/

int gpio_set_edge(unsigned int gpio, char *edge)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		return fd;
	}
	write(fd, edge, strlen(edge) + 1);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_poll
 * parameter gpio: gpio number to poll interrupt
 * parameter timeout: allows intermediate polling of the line
 ****************************************************************/

int gpio_poll(int gpio,int timeout)
{
	int ret;
	char rdbuf[MAX_BUF];
	char fn[MAX_BUF];
	struct pollfd pfd;

	snprintf(fn, MAX_BUF-1, "/sys/class/gpio/gpio%d/value", gpio);
	pfd.fd=open(fn, O_RDONLY);
	if(pfd.fd<0) {
		perror(fn);
		return 0;
	}
	// read value gpio to clear pending irq bit
	read(pfd.fd, rdbuf, MAX_BUF-1);
	printf("Level ignition pin is currently: %s", rdbuf);
	pfd.events=POLLPRI;

	while(1) {
		memset(rdbuf, 0x00, MAX_BUF);
		lseek(pfd.fd, 0, SEEK_SET);
		ret=poll(&pfd, 1, timeout);
		if(ret<0) {
			perror("poll()");
			close(pfd.fd);
			return -1;
		}
		if(ret==0) {
			ret=read(pfd.fd, rdbuf, MAX_BUF-1);
			if(ret<0) {
				perror("read()");
			}

			printf("Level ignition pin is currently %s", rdbuf);
			continue;
		}

		ret=read(pfd.fd, rdbuf, MAX_BUF-1);
		if(ret<0) {
			perror("read()");
			return -1;
		}

		printf("Level ignition pin changed level: %s", rdbuf);
		break;
	}
	close(pfd.fd);

	// please debounce signal to decrease noise influence
	return atol(rdbuf);
}


int main(int argc,char *argv[])
{
	int level;
	unsigned int  ignition_on;
	int system_boot_condition;
	int value;
	int i;

	if(argc == 2) {
		system_boot_condition =  atol(argv[1]);
		if( system_boot_condition & CG_SYSTEM_ARM_IGNITION_SENSE) {
			printf("Cloudgate will boot after shutdown on ignition sense arm\n");
		} else {
			printf("Cloudgate will not boot after shutdown on ignition sense arm\n");
		}
	}
	else {
		fprintf(stderr, "Usage: example_system_power_down  0|1\n");
		fprintf(stderr, "0: Cloudgate will not boot after shutdown on ignition sense arm\n");
		fprintf(stderr, "1: Cloudgate will boot after shutdown on ignition sense arm\n");
		return -1;
	}

	cg_init("example_system_boot_condition");

	cg_get_api_level(&level);
	printf("SDK API level: %d\n", level);

	if( gpio_export(IGNITION_SENSE_PIN) != 0) {
		fprintf(stderr, "Can't export gpio%d (ignition sense pin)\n", IGNITION_SENSE_PIN);
		goto error;
	}

	if (gpio_get_value(IGNITION_SENSE_PIN, &ignition_on) != 0) {
		fprintf(stderr, "Can't not read value of gpio%d (ignition sense pin)\n", IGNITION_SENSE_PIN);
		goto error;
	}

	// check ignition is on
	if (ignition_on == 0) {
		if( gpio_set_edge (IGNITION_SENSE_PIN, "rising") != 0) {
			fprintf(stderr, "Can't set interrupt of gpio%d (ignition sense pin) to rising edge\n", IGNITION_SENSE_PIN);
			goto error;
		}
		printf("Ignition is off, waiting until ignition is switched on...\n");
		value = gpio_poll(IGNITION_SENSE_PIN,5000);
		if(value < 0) {
			fprintf(stderr, "poll gpio%d (ignition sense pin) failed\n", IGNITION_SENSE_PIN);
			goto error;
		}

	}

	printf("Ignition is on, waiting until ignition is switched off...\n");

	if( gpio_set_edge(IGNITION_SENSE_PIN, "falling") != 0 ) {
		fprintf(stderr, "Can't set interrupt of gpio%d (ignition sense pin) to falling edge\n", IGNITION_SENSE_PIN);
		goto error;

	}

	value = gpio_poll(IGNITION_SENSE_PIN,5000);
	if(value < 0) {
		fprintf(stderr, "poll gpio%d (ignition sense pin) failed\n", IGNITION_SENSE_PIN);
		goto error;
	}

	if(value ) {
		fprintf(stderr, "expected value pin gpio%d (ignition sense pin): 0, but value 1 is found\n", IGNITION_SENSE_PIN);
		goto error;
	}
	printf("Ignition is off, shutdown the system...\n");

	if(cg_system_power_down(system_boot_condition & CG_SYSTEM_ARM_IGNITION_SENSE) == CG_STATUS_OK) {
		printf("performing system shutdown ...\n");
		cg_deinit();
		printf ("printing * every second, for 15 seconds to see system going down:");
		fflush(stdout);
		for (i = 0;i < 15; i++) {
			sleep(1);
			fprintf(stdout,"* ");
			fflush(stdout);
		}
		fprintf(stdout,"\n");
		if( gpio_unexport(IGNITION_SENSE_PIN) != 0) {
			fprintf(stderr, "Can't unexport gpio%d (ignition sense pin)\n", IGNITION_SENSE_PIN);
		}
		cg_deinit();
		return 0;
	} else {
		fprintf(stderr,"Can't execute system shutdown\n");
		goto error;
	}
error:
	if( gpio_unexport(IGNITION_SENSE_PIN) != 0) {
		fprintf(stderr, "Can't unexport gpio%d (ignition sense pin)\n", IGNITION_SENSE_PIN);
	}
	cg_deinit();

	return -1;
}
