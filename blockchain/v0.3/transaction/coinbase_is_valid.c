#include <string.h>
#include <stdlib.h>

#include "transaction.h"

/**
 * coinbase_is_valid - Checks whether a coinbase transaction is valid
 *
 * @coinbase:    Pointer to the coinbase transaction to verify
 * @block_index: Index of the Block the coinbase transaction belongs to
 *
 * Return: 1 if valid, 0 otherwise
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	tx_in_t *in;
	tx_out_t *out;
	uint8_t zeroes[SHA256_DIGEST_LENGTH];
	uint8_t zero_sig[72];

	if (!coinbase)
		return (0);
	transaction_hash(coinbase, hash_buf);
	if (memcmp(hash_buf, coinbase->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	if (llist_size(coinbase->inputs) != 1)
		return (0);
	if (llist_size(coinbase->outputs) != 1)
		return (0);
	in = (tx_in_t *)llist_get_node_at(coinbase->inputs, 0);
	out = (tx_out_t *)llist_get_node_at(coinbase->outputs, 0);
	memset(zeroes, 0, SHA256_DIGEST_LENGTH);
	memset(zero_sig, 0, 72);
	if (memcmp(in->block_hash, zeroes, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	if (memcmp(in->tx_id, zeroes, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	if (memcmp(in->tx_out_hash, &block_index, sizeof(block_index)) != 0)
		return (0);
	if (in->sig.len != 0)
		return (0);
	if (memcmp(in->sig.sig, zero_sig, 72) != 0)
		return (0);
	if (out->amount != COINBASE_AMOUNT)
		return (0);
	return (1);
}
