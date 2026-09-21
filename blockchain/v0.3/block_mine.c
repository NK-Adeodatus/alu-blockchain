#include <string.h>

#include "blockchain.h"


/**
 * block_mine - Mines a Block to find a valid hash matching the difficulty
 *
 * @block: Pointer to the Block to mine
 *
 * This function increments the Block's nonce until the resulting hash
 * matches the required difficulty.
 */
void block_mine(block_t *block)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];

	while (1)
	{
		block_hash(block, hash);
		if (hash_matches_difficulty(hash, block->info.difficulty))
		{
			memcpy(block->hash, hash, SHA256_DIGEST_LENGTH);
			return;
		}
		block->info.nonce++;
	}
}
