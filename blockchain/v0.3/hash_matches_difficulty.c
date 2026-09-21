#include "blockchain.h"

/**
 * hash_matches_difficulty - Checks whether a hash matches a given difficulty
 *
 * @hash:       Hash to check
 * @difficulty: Minimum difficulty the hash must match
 *
 * Return: 1 if the hash matches the difficulty, 0 otherwise
 *
 * The hash matches the difficulty if its first @difficulty bits are 0.
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
	uint32_t difficulty)
{
	uint32_t full_bytes;
	uint32_t remaining;
	uint32_t i;

	full_bytes = difficulty / 8;
	remaining = difficulty % 8;

	for (i = 0; i < full_bytes; i++)
	{
		if (hash[i] != 0)
			return (0);
	}
	if (remaining && full_bytes < SHA256_DIGEST_LENGTH)
	{
		if (hash[full_bytes] >> (8 - remaining))
			return (0);
	}
	return (1);
}
