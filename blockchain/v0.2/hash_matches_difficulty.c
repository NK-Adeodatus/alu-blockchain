#include "blockchain.h"

/**
 * hash_matches_difficulty - checks whether a hash matches a given difficulty
 * @hash: hash to check
 * @difficulty: minimum number of leading zero bits the hash must have
 *
 * Return: 1 if the difficulty is respected, 0 otherwise
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
			    uint32_t difficulty)
{
	uint32_t zeros = 0;
	uint8_t mask;
	size_t i;

	for (i = 0; i < SHA256_DIGEST_LENGTH && zeros < difficulty; i++)
	{
		for (mask = 0x80; mask && zeros < difficulty; mask >>= 1)
		{
			if (hash[i] & mask)
				return (0);
			zeros++;
		}
	}
	return (zeros >= difficulty);
}
