#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "blockchain.h"

/**
 * block_create - Creates a new Block and initialises it
 *
 * @prev:     Pointer to the previous Block in the Blockchain
 * @data:     Memory area to duplicate in the Block's data
 * @data_len: Number of bytes to duplicate from @data
 *
 * Return: Pointer to the allocated Block, or NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
	uint32_t data_len)
{
	block_t		*block;
	uint32_t	len;

	block = calloc(1, sizeof(*block));
	if (!block)
		return (NULL);
	block->info.index = prev->info.index + 1;
	block->info.difficulty = 0;
	block->info.nonce = 0;
	block->info.timestamp = (uint64_t)time(NULL);
	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
	len = data_len > BLOCKCHAIN_DATA_MAX ? BLOCKCHAIN_DATA_MAX : data_len;
	memcpy(block->data.buffer, data, len);
	block->data.len = len;
	block->transactions = llist_create(MT_SUPPORT_FALSE);
	return (block);
}
