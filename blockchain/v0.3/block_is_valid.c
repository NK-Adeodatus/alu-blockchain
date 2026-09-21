#include <string.h>

#include "blockchain.h"

/**
 * check_tx - Validates a non-coinbase transaction
 *
 * @node: Current transaction
 * @idx:  Index (skip coinbase at 0)
 * @arg:  all_unspent list
 *
 * Return: 0 if valid, -1 otherwise
 */
static int check_tx(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *tx = (transaction_t *)node;
	llist_t *all_unspent = (llist_t *)arg;

	if (idx == 0)
		return (0);
	if (!transaction_is_valid(tx, all_unspent))
		return (-1);
	return (0);
}

/**
 * block_is_valid - Verifies that a Block is valid
 *
 * @block:       Pointer to the Block to check
 * @prev_block:  Pointer to the previous Block, or NULL if @block is first
 * @all_unspent: List of all unspent transaction outputs
 *
 * Return: 0 if the Block is valid, -1 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block,
	llist_t *all_unspent)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];
	static uint8_t const zero[SHA256_DIGEST_LENGTH];
	transaction_t *coinbase;

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
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (-1);
	if (!block->transactions || llist_size(block->transactions) < 1)
		return (-1);
	coinbase = llist_get_node_at(block->transactions, 0);
	if (!coinbase_is_valid(coinbase, block->info.index))
		return (-1);
	if (llist_for_each(block->transactions, check_tx, (void *)all_unspent))
		return (-1);
	return (0);
}
