//---------------------------------------------------------------------------
// Copyright (C) 2000 Dallas Semiconductor Corporation, All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// Except as contained in this notice, the name of Dallas Semiconductor
// shall not be used except as stated in the Dallas Semiconductor
// Branding Policy.
// ---------------------------------------------------------------------------
//
//  temp28.C - Module to read the DS18B20 - temperature measurement.
//
//  Version: 1.00
//
// ---------------------------------------------------------------------------
//
//
#include "ownet.h"
#include "temp10.h"

//----------------------------------------------------------------------
// Read the temperature of a DS18B20
//
// 'portnum'     - number 0 to MAX_PORTNUM-1.  This number was provided to
//                 OpenCOM to indicate the port number.
// 'SerialNum'   - Serial Number of DS1920/DS1820 to read temperature from
// 'Temp '       - pointer to variable where that temperature will be
//                 returned
//
// Returns: TRUE(1)  temperature has been read and verified
//          FALSE(0) could not read the temperature
//
int ReadTemperature(int portnum, uchar *SerialNum, float *Temp)
{
	uchar rt=FALSE;
	uchar rx_pad[9], lastcrc8;
	int i, loop=0;

	*Temp = 0.0;
	// set the device serial number to the counter device
	owSerialNum(portnum,SerialNum,FALSE);

	for (loop = 0; loop < 2; loop ++)
	{
		// access the device
		if (owAccess(portnum))
		{
			// send the convert command
			if (!owWriteBytePower(portnum,0x44))
				return FALSE;

			// sleep for 1 second
			msDelay(1000);

			// turn off the 1-Wire Net strong pull-up
			if (owLevel(portnum,MODE_NORMAL) != MODE_NORMAL)
				return FALSE;

			// access the device
			if (owAccess(portnum))
			{
				// read scratchpad command
				if (!owTouchByte(portnum, 0xBE))
					return FALSE;

				// now read bytes
				for (i = 0; i < 9; i++)
					rx_pad[i] = owReadByte(portnum);

				// perform the CRC8 on the first 8 bytes
				setcrc8(portnum,0);
				for (i = 0; i < 8; i++)
					lastcrc8 = docrc8(portnum,rx_pad[i]);

				// Verify CRC
				if (lastcrc8 == rx_pad[8]) {
					short t  = (rx_pad[1] << 8) | rx_pad[0];
					// scale temp (12 bit accuracy)
					*Temp = t / 16.0;
					rt = TRUE;
					break;
				}
			}
		}
	}
	// return the result flag rt
	return rt;
}

