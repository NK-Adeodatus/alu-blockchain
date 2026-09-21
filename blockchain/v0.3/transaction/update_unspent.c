#include <stdlib.h>
#include <string.h>

#include "transaction.h"

/**
 * is_input_match - Checks if a tx_in references a given unspent output
 *
 * @node: Current tx_in
 * @idx:  Index (unused)
 * @arg:  unspent_tx_out_t pointer
 *
 * Return: 1 if match, 0 otherwise
 */
static int is_input_match(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *in = (tx_in_t *)node;
	unspent_tx_out_t *u = (unspent_tx_out_t *)arg;

	(void)idx;
	if (!memcmp(in->tx_out_hash, u->out.hash, SHA256_DIGEST_LENGTH) &&
	    !memcmp(in->block_hash, u->block_hash, SHA256_DIGEST_LENGTH) &&
	    !memcmp(in->tx_id, u->tx_id, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}

/**
 * add_tx_outputs - Adds all outputs of a transaction to new_unspent list
 *
 * @node: Current transaction
 * @idx:  Index (unused)
 * @arg:  Pointer to add_ctx_t
 *
 * Return: 0 to continue
 */
static int add_tx_outputs(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *tx = (transaction_t *)node;
	add_ctx_t *ctx = (add_ctx_t *)arg;
	int i, nout;
	tx_out_t *out;
	unspent_tx_out_t *unspent;

	(void)idx;
	nout = llist_size(tx->outputs);
	for (i = 0; i < nout; i++)
	{
		out = llist_get_node_at(tx->outputs, i);
		unspent = unspent_tx_out_create(ctx->block_hash, tx->id, out);
		if (unspent)
			llist_add_node(ctx->new_list, unspent, ADD_NODE_REAR);
	}
	return (0);
}

/**
 * copy_if_unspent - Copies an unspent output if not consumed by any input
 *
 * @node: Current unspent_tx_out_t
 * @idx:  Index (unused)
 * @arg:  Pointer to filter_ctx_t
 *
 * Return: 0 to continue
 */
static int copy_if_unspent(llist_node_t node, unsigned int idx, void *arg)
{
	unspent_tx_out_t *u = (unspent_tx_out_t *)node;
	filter_ctx_t *ctx = (filter_ctx_t *)arg;
	transaction_t *tx;
	unspent_tx_out_t *new_u;
	int i, ntx;

	(void)idx;
	ntx = llist_size(ctx->transactions);
	for (i = 0; i < ntx; i++)
	{
		tx = llist_get_node_at(ctx->transactions, i);
		if (llist_for_each(tx->inputs, is_input_match, u))
			return (0);
	}
	new_u = malloc(sizeof(*new_u));
	if (!new_u)
		return (0);
	memcpy(new_u, u, sizeof(*new_u));
	llist_add_node(ctx->new_list, new_u, ADD_NODE_REAR);
	return (0);
}

/**
 * update_unspent - Updates the list of all unspent transaction outputs
 *
 * @transactions: List of validated transactions
 * @block_hash:   Hash of the validated Block
 * @all_unspent:  Current list of unspent transaction outputs
 *
 * Return: New list of unspent transaction outputs
 */
llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	llist_t *new_list;
	filter_ctx_t fctx;
	add_ctx_t actx;

	new_list = llist_create(MT_SUPPORT_FALSE);
	if (!new_list)
		return (NULL);

	fctx.transactions = transactions;
	fctx.new_list = new_list;
	llist_for_each(all_unspent, copy_if_unspent, &fctx);

	actx.block_hash = block_hash;
	actx.new_list = new_list;
	llist_for_each(transactions, add_tx_outputs, &actx);

	llist_destroy(all_unspent, 1, free);
	return (new_list);
}
