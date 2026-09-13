#include <stdlib.h>
#include <string.h>

#include "transaction.h"

/**
 * collect_unspent - Collects unspent outputs whose pub key matches sender
 *
 * @node: Current unspent transaction output
 * @idx:  Index (unused)
 * @arg:  Pointer to collect_tx_t context
 *
 * Return: 0 to continue, -1 on error
 */
static int collect_unspent(llist_node_t node, unsigned int idx, void *arg)
{
	unspent_tx_out_t *u = (unspent_tx_out_t *)node;
	collect_tx_t *ctx = (collect_tx_t *)arg;
	tx_in_t *in;

	(void)idx;
	if (ctx->done)
		return (0);
	if (memcmp(u->out.pub, ctx->sender_pub, EC_PUB_LEN) != 0)
		return (0);
	in = tx_in_create(u);
	if (!in)
		return (-1);
	llist_add_node(ctx->inputs, in, ADD_NODE_REAR);
	ctx->total += u->out.amount;
	if (ctx->total >= ctx->amount)
		ctx->done = 1;
	return (0);
}

/**
 * sign_input - Signs a single transaction input
 *
 * @node: Current transaction input
 * @idx:  Index (unused)
 * @arg:  Pointer to sign_ctx_t context
 *
 * Return: 0 on success, -1 on failure
 */
static int sign_input(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *in = (tx_in_t *)node;
	sign_ctx_t *ctx = (sign_ctx_t *)arg;

	(void)idx;
	if (!tx_in_sign(in, ctx->tx_id, ctx->sender, ctx->all_unspent))
	{
		ctx->valid = 0;
		return (-1);
	}
	return (0);
}

/**
 * transaction_create - Creates a transaction
 *
 * @sender:      Private key of the transaction sender
 * @receiver:    Public key of the transaction receiver
 * @amount:      Amount to send
 * @all_unspent: List of all unspent transaction outputs
 *
 * Return: Pointer to the created transaction, or NULL upon failure
 */
transaction_t *transaction_create(EC_KEY const *sender,
	EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent)
{
	transaction_t *tx;
	collect_tx_t cctx;
	sign_ctx_t sctx;
	uint8_t receiver_pub[EC_PUB_LEN];
	tx_out_t *out;

	if (!sender || !receiver || !all_unspent)
		return (NULL);
	ec_to_pub(sender, cctx.sender_pub);
	cctx.inputs = llist_create(MT_SUPPORT_FALSE);
	cctx.total = 0;
	cctx.amount = amount;
	cctx.done = 0;
	llist_for_each(all_unspent, collect_unspent, &cctx);
	if (cctx.total < amount)
	{
		llist_destroy(cctx.inputs, 1, free);
		return (NULL);
	}
	tx = calloc(1, sizeof(*tx));
	if (!tx)
	{
		llist_destroy(cctx.inputs, 1, free);
		return (NULL);
	}
	tx->inputs = cctx.inputs;
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(receiver, receiver_pub);
	out = tx_out_create(amount, receiver_pub);
	llist_add_node(tx->outputs, out, ADD_NODE_REAR);
	if (cctx.total > amount)
	{
		out = tx_out_create(cctx.total - amount, cctx.sender_pub);
		llist_add_node(tx->outputs, out, ADD_NODE_REAR);
	}
	transaction_hash(tx, tx->id);
	sctx.tx_id = tx->id;
	sctx.sender = sender;
	sctx.all_unspent = all_unspent;
	sctx.valid = 1;
	llist_for_each(tx->inputs, sign_input, &sctx);
	if (!sctx.valid)
	{
		llist_destroy(tx->inputs, 1, free);
		llist_destroy(tx->outputs, 1, free);
		free(tx);
		return (NULL);
	}
	return (tx);
}
