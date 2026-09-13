#include <openssl/sha.h>

#include "blockchain.h"

/**
 * block_hash - Computes the hash of a Block
 *
 * @block:    Pointer to the Block to hash
 * @hash_buf: Buffer in which to store the resulting hash
 *
 * The hash is computed over the Block's info and data fields.
 * block->hash is left unchanged.
 *
 * Return: Pointer to @hash_buf
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX	ctx;

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, &block->info, sizeof(block->info));
	SHA256_Update(&ctx, block->data.buffer, block->data.len);
	SHA256_Final(hash_buf, &ctx);
	return (hash_buf);
}
