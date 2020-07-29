#ifndef libuqmi_unaligned_h
#define libuqmi_unaligned_h

#include <stdlib.h>
#include <inttypes.h>

/** See Linux headers unaligned.h */

static inline uint8_t __get_unaligned_le8(const uint8_t *p)
{
	return p[0];
}

static inline uint16_t __get_unaligned_le16(const uint8_t *p)
{
	return p[0] | p[1] << 8;
}

static inline uint32_t __get_unaligned_le32(const uint8_t *p)
{
	return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
}

static inline uint64_t __get_unaligned_le64(const uint8_t *p)
{
	return (uint64_t) __get_unaligned_le32(p + 4) << 32 |
		__get_unaligned_le32(p);
}

#endif
