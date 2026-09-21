#include <stdio.h>
#include <stdint.h>

#include "blockchain.h"

/**
 * write_tx_input - Writes a transaction input to a file
 *
 * @node: Current tx_in
 * @idx:  Index (unused)
 * @arg:  FILE pointer
 *
 * Return: 0 to continue
 */
static int write_tx_input(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *in = (tx_in_t *)node;
	FILE *f = (FILE *)arg;

	(void)idx;
	fwrite(in->block_hash, SHA256_DIGEST_LENGTH, 1, f);
	fwrite(in->tx_id, SHA256_DIGEST_LENGTH, 1, f);
	fwrite(in->tx_out_hash, SHA256_DIGEST_LENGTH, 1, f);
	fwrite(in->sig.sig, SIG_MAX_LEN, 1, f);
	fwrite(&in->sig.len, 1, 1, f);
	return (0);
}

/**
 * write_tx_output - Writes a transaction output to a file
 *
 * @node: Current tx_out
 * @idx:  Index (unused)
 * @arg:  FILE pointer
 *
 * Return: 0 to continue
 */
static int write_tx_output(llist_node_t node, unsigned int idx, void *arg)
{
	tx_out_t *out = (tx_out_t *)node;
	FILE *f = (FILE *)arg;

	(void)idx;
	fwrite(&out->amount, sizeof(out->amount), 1, f);
	fwrite(out->pub, EC_PUB_LEN, 1, f);
	fwrite(out->hash, SHA256_DIGEST_LENGTH, 1, f);
	return (0);
}

/**
 * write_transaction - Writes a transaction to a file
 *
 * @node: Current transaction
 * @idx:  Index (unused)
 * @arg:  FILE pointer
 *
 * Return: 0 to continue
 */
static int write_transaction(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *tx = (transaction_t *)node;
	FILE *f = (FILE *)arg;
	uint32_t nin, nout;

	(void)idx;
	nin = (uint32_t)llist_size(tx->inputs);
	nout = (uint32_t)llist_size(tx->outputs);
	fwrite(tx->id, SHA256_DIGEST_LENGTH, 1, f);
	fwrite(&nin, sizeof(nin), 1, f);
	fwrite(&nout, sizeof(nout), 1, f);
	llist_for_each(tx->inputs, write_tx_input, f);
	llist_for_each(tx->outputs, write_tx_output, f);
	return (0);
}

/**
 * blockchain_serialize - Serializes a Blockchain into a file
 *
 * @blockchain: Pointer to the Blockchain to serialize
 * @path:       Path to the output file (overwritten if it exists)
 *
 * Return: 0 on success, or -1 on failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE		*f;
	uint32_t	nb_blocks, nb_unspent;
	int		i, size, ntx;
	block_t		*block;
	unspent_tx_out_t *u;
	uint16_t	w = 0x0102;
	uint8_t		endian = *(uint8_t *)&w == 1 ? 2 : 1;
	int32_t		nb_tx;

	f = fopen(path, "wb");
	if (!f)
		return (-1);

	size = llist_size(blockchain->chain);
	nb_blocks = (uint32_t)size;
	nb_unspent = (uint32_t)llist_size(blockchain->unspent);

	fwrite("HBLK", 1, 4, f);
	fwrite("0.3", 1, 3, f);
	fwrite(&endian, 1, 1, f);
	fwrite(&nb_blocks, sizeof(nb_blocks), 1, f);
	fwrite(&nb_unspent, sizeof(nb_unspent), 1, f);

	for (i = 0; i < size; i++)
	{
		block = llist_get_node_at(blockchain->chain, i);
		fwrite(&block->info.index, sizeof(block->info.index), 1, f);
		fwrite(&block->info.difficulty, sizeof(block->info.difficulty), 1, f);
		fwrite(&block->info.timestamp, sizeof(block->info.timestamp), 1, f);
		fwrite(&block->info.nonce, sizeof(block->info.nonce), 1, f);
		fwrite(block->info.prev_hash, SHA256_DIGEST_LENGTH, 1, f);
		fwrite(&block->data.len, sizeof(block->data.len), 1, f);
		fwrite(block->data.buffer, block->data.len, 1, f);
		fwrite(block->hash, SHA256_DIGEST_LENGTH, 1, f);
		if (!block->transactions)
			nb_tx = -1;
		else
			nb_tx = (int32_t)llist_size(block->transactions);
		fwrite(&nb_tx, sizeof(nb_tx), 1, f);
		if (nb_tx > 0)
			llist_for_each(block->transactions, write_transaction, f);
	}

	ntx = llist_size(blockchain->unspent);
	for (i = 0; i < ntx; i++)
	{
		u = llist_get_node_at(blockchain->unspent, i);
		fwrite(u->block_hash, SHA256_DIGEST_LENGTH, 1, f);
		fwrite(u->tx_id, SHA256_DIGEST_LENGTH, 1, f);
		fwrite(&u->out.amount, sizeof(u->out.amount), 1, f);
		fwrite(u->out.pub, EC_PUB_LEN, 1, f);
		fwrite(u->out.hash, SHA256_DIGEST_LENGTH, 1, f);
	}

	fclose(f);
	return (0);
}
