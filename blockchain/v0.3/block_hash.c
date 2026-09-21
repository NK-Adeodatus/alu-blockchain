#include <openssl/sha.h>

#include "blockchain.h"

/**
 * update_ctx_with_tx - Adds a transaction ID to a SHA256 context
 *
 * @node: Current transaction
 * @idx:  Index (unused)
 * @arg:  Pointer to SHA256_CTX
 *
 * Return: 0 to continue
 */
static int update_ctx_with_tx(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *tx = (transaction_t *)node;
	SHA256_CTX *ctx = (SHA256_CTX *)arg;

	(void)idx;
	SHA256_Update(ctx, tx->id, SHA256_DIGEST_LENGTH);
	return (0);
}

/**
 * block_hash - Computes the hash of a Block
 *
 * @block:    Pointer to the Block to hash
 * @hash_buf: Buffer in which to store the resulting hash
 *
 * The hash is computed over the Block's info, data, and transactions.
 * block->hash is left unchanged.
 *
 * Return: Pointer to @hash_buf
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX ctx;

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, &block->info, sizeof(block->info));
	SHA256_Update(&ctx, block->data.buffer, block->data.len);
	if (block->transactions)
		llist_for_each(block->transactions, update_ctx_with_tx, &ctx);
	SHA256_Final(hash_buf, &ctx);
	return (hash_buf);
}
