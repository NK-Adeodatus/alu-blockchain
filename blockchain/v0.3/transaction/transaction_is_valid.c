#include <string.h>
#include <stdlib.h>

#include "transaction.h"

/**
 * find_u - Callback to find an unspent output by hash
 *
 * @node: Current unspent transaction output
 * @idx:  Index (unused)
 * @arg:  Pointer to find_unspent_t context
 *
 * Return: 1 if found, 0 to continue
 */
static int find_u(llist_node_t node, unsigned int idx, void *arg)
{
	unspent_tx_out_t *u = (unspent_tx_out_t *)node;
	find_unspent_t *ctx = (find_unspent_t *)arg;

	(void)idx;
	if (memcmp(u->out.hash, ctx->tx_out_hash, SHA256_DIGEST_LENGTH) == 0)
	{
		ctx->found = u;
		return (1);
	}
	return (0);
}

/**
 * validate_input - Validates a single transaction input
 *
 * @node: Current transaction input
 * @idx:  Index (unused)
 * @arg:  Pointer to valid_in_ctx_t context
 *
 * Return: 0 to continue, -1 on invalid
 */
static int validate_input(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *in = (tx_in_t *)node;
	valid_in_ctx_t *ctx = (valid_in_ctx_t *)arg;
	find_unspent_t fctx;
	EC_KEY *key;

	(void)idx;
	fctx.tx_out_hash = in->tx_out_hash;
	fctx.found = NULL;
	llist_for_each(ctx->all_unspent, find_u, &fctx);
	if (!fctx.found)
	{
		ctx->valid = 0;
		return (-1);
	}
	key = ec_from_pub(fctx.found->out.pub);
	if (!key || !ec_verify(key, ctx->tx_id,
		SHA256_DIGEST_LENGTH, &in->sig))
	{
		ctx->valid = 0;
		return (-1);
	}
	ctx->in_total += fctx.found->out.amount;
	return (0);
}

/**
 * transaction_is_valid - Checks whether a transaction is valid
 *
 * @transaction: Pointer to the transaction to verify
 * @all_unspent: List of all unspent transaction outputs
 *
 * Return: 1 if valid, 0 otherwise
 */
int transaction_is_valid(transaction_t const *transaction,
	llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	uint32_t out_total;
	valid_in_ctx_t ctx;
	tx_out_t *out;
	int i, nout;

	if (!transaction || !all_unspent)
		return (0);
	transaction_hash(transaction, hash_buf);
	if (memcmp(hash_buf, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	ctx.all_unspent = all_unspent;
	ctx.tx_id = transaction->id;
	ctx.in_total = 0;
	ctx.valid = 1;
	llist_for_each(transaction->inputs, validate_input, &ctx);
	if (!ctx.valid)
		return (0);
	out_total = 0;
	nout = llist_size(transaction->outputs);
	for (i = 0; i < nout; i++)
	{
		out = (tx_out_t *)llist_get_node_at(transaction->outputs, i);
		out_total += out->amount;
	}
	return (ctx.in_total == out_total);
}
