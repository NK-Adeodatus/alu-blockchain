#include "endianness.h"

/**
 * _get_endianness - Returns the endianness of the current system
 *
 * Return: LITTLE_ENDIAN, BIG_ENDIAN or UNKNOWN_ENDIAN
 */
int _get_endianness(void)
{
	union
	{
		uint16_t	s;
		uint8_t		c[2];
	} u;

	u.s = 0x0102;
	if (u.c[0] == 1)
		return (BIG_ENDIAN);
	if (u.c[0] == 2)
		return (LITTLE_ENDIAN);
	return (UNKNOWN_ENDIAN);
}
