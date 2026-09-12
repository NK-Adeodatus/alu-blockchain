#ifndef ENDIANNESS_H
#define ENDIANNESS_H

/**
 * LITTLE_ENDIAN - Little-endian byte order
 * BIG_ENDIAN    - Big-endian byte order
 * UNKNOWN_ENDIAN - Unknown byte order
 */
#define LITTLE_ENDIAN	0
#define BIG_ENDIAN	1
#define UNKNOWN_ENDIAN	-1

int	_get_endianness(void);

#endif /* ENDIANNESS_H */
