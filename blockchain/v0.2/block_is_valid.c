#include <string.h>

#include "blockchain.h"

/**
 * block_is_valid - Verifies that a Block is valid
 *
 * @block:      Pointer to the Block to check
 * @prev_block: Pointer to the previous Block, or NULL if @block is first
 *
 * Return: 0 if the Block is valid, -1 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t		hash[SHA256_DIGEST_LENGTH];
	static uint8_t	const zero[SHA256_DIGEST_LENGTH];

	if (!block)
		return (-1);
	if ((block->info.index == 0) != (prev_block == NULL))
		return (-1);
	if (block->info.index == 0)
	{
		if (memcmp(block->info.prev_hash, zero, SHA256_DIGEST_LENGTH) ||
			block->data.len != GENESIS_DATA_LEN ||
			memcmp(block->data.buffer, GENESIS_DATA, GENESIS_DATA_LEN) ||
			memcmp(block->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH))
			return (-1);
		return (0);
	}
	if (block->info.index != prev_block->info.index + 1)
		return (-1);
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (-1);
	block_hash(prev_block, hash);
	if (memcmp(hash, prev_block->hash, SHA256_DIGEST_LENGTH) ||
		memcmp(hash, block->info.prev_hash, SHA256_DIGEST_LENGTH))
		return (-1);
	block_hash(block, hash);
	if (memcmp(hash, block->hash, SHA256_DIGEST_LENGTH))
		return (-1);
	return (0);
}
