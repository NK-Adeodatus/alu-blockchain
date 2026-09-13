#include <stdlib.h>
#include <string.h>

#include "transaction.h"

/**
 * coinbase_create - Creates a coinbase transaction
 *
 * @receiver:    Public key of the miner receiving coinbase coins
 * @block_index: Index of the Block the coinbase transaction belongs to
 *
 * Return: Pointer to the created transaction, or NULL upon failure
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *tx;
	tx_in_t *in;
	tx_out_t *out;
	uint8_t receiver_pub[EC_PUB_LEN];

	if (!receiver)
		return (NULL);
	tx = calloc(1, sizeof(*tx));
	if (!tx)
		return (NULL);
	in = calloc(1, sizeof(*in));
	if (!in)
	{
		free(tx);
		return (NULL);
	}
	memcpy(in->tx_out_hash, &block_index, sizeof(block_index));
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	llist_add_node(tx->inputs, in, ADD_NODE_REAR);
	ec_to_pub(receiver, receiver_pub);
	out = tx_out_create(COINBASE_AMOUNT, receiver_pub);
	if (!out)
	{
		llist_destroy(tx->inputs, 1, free);
		llist_destroy(tx->outputs, 0, NULL);
		free(tx);
		return (NULL);
	}
	llist_add_node(tx->outputs, out, ADD_NODE_REAR);
	transaction_hash(tx, tx->id);
	return (tx);
}
