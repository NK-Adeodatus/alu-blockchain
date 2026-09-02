#include <stdio.h>
#include <stdint.h>

/**
 * _print_hex_buffer - Prints the content of a buffer as hex
 *
 * @buf: Pointer to the buffer to print
 * @len: Length of the buffer
 */
void _print_hex_buffer(uint8_t const *buf, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		printf("%02x", buf[i]);
}
