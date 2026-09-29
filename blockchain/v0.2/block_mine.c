#include "blockchain.h"

/**
 * block_mine - mines a Block so its hash matches its difficulty
 * @block: pointer to the Block to mine
 */
void block_mine(block_t *block)
{
	if (!block)
		return;

	block->info.nonce = 0;
	block_hash(block, block->hash);
	while (!hash_matches_difficulty(block->hash, block->info.difficulty))
	{
		block->info.nonce++;
		block_hash(block, block->hash);
	}
}
