#ifndef __CORTEX_OWIRE_IOCTL__
#define __CORTEX_OWIRE_IOCTL__

#define MAGIC_OW                    'O'

#define IOCTL_CORTEX_OW_TOUCH_RESET _IOWR(MAGIC_OW, 0x0A, char*)
#define IOCTL_CORTEX_OW_TOUCH_BIT   _IOWR(MAGIC_OW, 0x0B, char*)
#define IOCTL_CORTEX_OW_TOUCH_BYTE  _IOWR(MAGIC_OW, 0x0C, char*)
#define IOCTL_CORTEX_OW_SPU         _IOWR(MAGIC_OW, 0x0D, char*)

#endif


