#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "transaction.h"

/**
 * struct find_unspent_s - Helper struct for finding unspent tx out
 *
 * @tx_out_hash: Hash to search for
 * @found: Found unspent tx out
 */
typedef struct find_unspent_s
{
	uint8_t const	*tx_out_hash;
	unspent_tx_out_t *found;
} find_unspent_t;

/**
 * find_unspent - llist_for_each callback to find matching unspent output
 *
 * @node: Current unspent tx output
 * @idx: Index (unused)
 * @arg: Pointer to find_unspent_t
 *
 * Return: 1 if found, 0 to continue
 */
static int find_unspent(llist_node_t node, unsigned int idx, void *arg)
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
 * tx_in_sign - Signs a transaction input given the transaction id
 *
 * @in: Pointer to the transaction input to sign
 * @tx_id: ID of the transaction the input is stored in
 * @sender: Private key of the receiver of the coins
 * @all_unspent: List of all unspent transaction outputs
 *
 * Return: Pointer to the resulting signature, or NULL upon failure
 */
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender, llist_t *all_unspent)
{
	uint8_t sender_pub[EC_PUB_LEN];
	find_unspent_t ctx;

	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);

	ctx.tx_out_hash = in->tx_out_hash;
	ctx.found = NULL;
	llist_for_each(all_unspent, find_unspent, &ctx);
	if (!ctx.found)
	{
		fprintf(stderr, "tx_in_sign: unspent output not found\n");
		return (NULL);
	}

	ec_to_pub(sender, sender_pub);
	if (memcmp(sender_pub, ctx.found->out.pub, EC_PUB_LEN) != 0)
	{
		fprintf(stderr, "tx_in_sign: public key mismatch\n");
		return (NULL);
	}

	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}
