#include "CGHelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <libcg/cg_general.h>
#include <libcg/cg_device.h>

FILE *fp_since_epoch;
char *serialNumber;
bool initialized = false;

void init()
{
    if (!initialized)
    {
        fp_since_epoch = fopen("/sys/devices/platform/mxs-rtc.0/rtc/rtc0/since_epoch", "r");

        serialNumber = (char *)malloc(256);

        initialized = true;
    }
}

void deinit()
{
    if (initialized)
    {
        fclose(fp_since_epoch);

        free(serialNumber);

        initialized = false;
    }
}

unsigned int getUnixTime()
{
    init();

    unsigned int buff;

    fscanf(fp_since_epoch, "%u", &buff);
    fseek(fp_since_epoch, 0, SEEK_SET);

    return buff;
}

char * getSerialNumber()
{
    init();

    cg_get_serial_number(&serialNumber);

    return serialNumber;
}
