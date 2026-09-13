#include "transaction.h"

/**
 * unspent_tx_out_create - Allocates and initializes an unspent
 * transaction output structure
 *
 * @block_hash: Hash of the Block where the referenced transaction
 * output is located
 * @tx_id: Hash of a transaction in the Block block_hash, where the
 * referenced transaction output is located
 * @out: Points to the referenced transaction output
 *
 * Return: Pointer to the created unspent transaction output, or NULL
 * upon failure
 */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{
	unspent_tx_out_t *unspent;

	if (!out)
		return (NULL);

	unspent = calloc(1, sizeof(*unspent));
	if (!unspent)
		return (NULL);

	memcpy(unspent->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(unspent->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	memcpy(&unspent->out, out, sizeof(unspent->out));

	return (unspent);
}
