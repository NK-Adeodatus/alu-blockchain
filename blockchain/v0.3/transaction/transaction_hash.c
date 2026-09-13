#include <stdlib.h>
#include <string.h>

#include "transaction.h"

/**
 * collect_ins - Collects input hashes into buffer
 *
 * @node: Transaction input node
 * @idx: Index (unused)
 * @arg: Pointer to current buffer position
 *
 * Return: 0 on success
 */
static int collect_ins(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *in = (tx_in_t *)node;
	uint8_t **buf = (uint8_t **)arg;

	(void)idx;
	memcpy(*buf, in->block_hash, SHA256_DIGEST_LENGTH);
	*buf += SHA256_DIGEST_LENGTH;
	memcpy(*buf, in->tx_id, SHA256_DIGEST_LENGTH);
	*buf += SHA256_DIGEST_LENGTH;
	memcpy(*buf, in->tx_out_hash, SHA256_DIGEST_LENGTH);
	*buf += SHA256_DIGEST_LENGTH;

	return (0);
}

/**
 * collect_outs - Collects output hashes into buffer
 *
 * @node: Transaction output node
 * @idx: Index (unused)
 * @arg: Pointer to current buffer position
 *
 * Return: 0 on success
 */
static int collect_outs(llist_node_t node, unsigned int idx, void *arg)
{
	tx_out_t *out = (tx_out_t *)node;
	uint8_t **buf = (uint8_t **)arg;

	(void)idx;
	memcpy(*buf, out->hash, SHA256_DIGEST_LENGTH);
	*buf += SHA256_DIGEST_LENGTH;

	return (0);
}

/**
 * transaction_hash - Computes the ID (hash) of a transaction
 *
 * @transaction: Pointer to the transaction to hash
 * @hash_buf: Buffer in which to store the computed hash
 *
 * Return: Pointer to hash_buf
 */
uint8_t *transaction_hash(transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int nin, nout;
	size_t buflen;
	uint8_t *buf, *p;

	if (!transaction || !hash_buf)
		return (NULL);

	nin = llist_size(transaction->inputs);
	nout = llist_size(transaction->outputs);
	buflen = (size_t)nin * SHA256_DIGEST_LENGTH * 3 +
		(size_t)nout * SHA256_DIGEST_LENGTH;

	buf = malloc(buflen);
	if (!buf)
		return (NULL);

	p = buf;
	llist_for_each(transaction->inputs, collect_ins, &p);
	llist_for_each(transaction->outputs, collect_outs, &p);

	sha256((int8_t const *)buf, buflen, hash_buf);
	free(buf);

	return (hash_buf);
}
