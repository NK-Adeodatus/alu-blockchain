#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

/**
 * read_tx_input - Reads one transaction input from file
 *
 * @f: File to read from
 *
 * Return: Pointer to allocated tx_in_t or NULL on failure
 */
static tx_in_t *read_tx_input(FILE *f)
{
	tx_in_t *in;

	in = calloc(1, sizeof(*in));
	if (!in)
		return (NULL);
	fread(in->block_hash, SHA256_DIGEST_LENGTH, 1, f);
	fread(in->tx_id, SHA256_DIGEST_LENGTH, 1, f);
	fread(in->tx_out_hash, SHA256_DIGEST_LENGTH, 1, f);
	fread(in->sig.sig, SIG_MAX_LEN, 1, f);
	fread(&in->sig.len, 1, 1, f);
	return (in);
}

/**
 * read_tx_output - Reads one transaction output from file
 *
 * @f: File to read from
 *
 * Return: Pointer to allocated tx_out_t or NULL on failure
 */
static tx_out_t *read_tx_output(FILE *f)
{
	tx_out_t *out;

	out = calloc(1, sizeof(*out));
	if (!out)
		return (NULL);
	fread(&out->amount, sizeof(out->amount), 1, f);
	fread(out->pub, EC_PUB_LEN, 1, f);
	fread(out->hash, SHA256_DIGEST_LENGTH, 1, f);
	return (out);
}

/**
 * read_transaction - Reads one transaction from file
 *
 * @f: File to read from
 *
 * Return: Pointer to allocated transaction_t or NULL on failure
 */
static transaction_t *read_transaction(FILE *f)
{
	transaction_t *tx;
	uint32_t nin, nout, j;
	tx_in_t *in;
	tx_out_t *out;

	tx = calloc(1, sizeof(*tx));
	if (!tx)
		return (NULL);
	fread(tx->id, SHA256_DIGEST_LENGTH, 1, f);
	fread(&nin, sizeof(nin), 1, f);
	fread(&nout, sizeof(nout), 1, f);
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	for (j = 0; j < nin; j++)
	{
		in = read_tx_input(f);
		if (in)
			llist_add_node(tx->inputs, in, ADD_NODE_REAR);
	}
	for (j = 0; j < nout; j++)
	{
		out = read_tx_output(f);
		if (out)
			llist_add_node(tx->outputs, out, ADD_NODE_REAR);
	}
	return (tx);
}

/**
 * blockchain_deserialize - Deserializes a Blockchain from a file
 *
 * @path: Path to the file to deserialize from
 *
 * Return: Pointer to the deserialized Blockchain, or NULL on failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE		*f;
	blockchain_t	*bc;
	block_t		*block;
	uint32_t	i, nb_blocks, nb_unspent;
	uint8_t		hdr[8];
	int32_t		nb_tx;
	transaction_t	*tx;
	unspent_tx_out_t *u;
	uint32_t	j;

	f = fopen(path, "rb");
	if (!f)
		return (NULL);
	if (fread(hdr, 1, 8, f) != 8 ||
		memcmp(hdr, "HBLK", 4) != 0 ||
		memcmp(hdr + 4, "0.3", 3) != 0)
	{
		fclose(f);
		return (NULL);
	}
	if (fread(&nb_blocks, sizeof(nb_blocks), 1, f) != 1 ||
		fread(&nb_unspent, sizeof(nb_unspent), 1, f) != 1)
	{
		fclose(f);
		return (NULL);
	}
	bc = malloc(sizeof(*bc));
	if (!bc)
	{
		fclose(f);
		return (NULL);
	}
	bc->chain = llist_create(MT_SUPPORT_FALSE);
	bc->unspent = llist_create(MT_SUPPORT_FALSE);
	for (i = 0; i < nb_blocks; i++)
	{
		block = calloc(1, sizeof(*block));
		if (!block)
			break;
		fread(&block->info.index, sizeof(block->info.index), 1, f);
		fread(&block->info.difficulty,
			sizeof(block->info.difficulty), 1, f);
		fread(&block->info.timestamp,
			sizeof(block->info.timestamp), 1, f);
		fread(&block->info.nonce, sizeof(block->info.nonce), 1, f);
		fread(block->info.prev_hash, SHA256_DIGEST_LENGTH, 1, f);
		fread(&block->data.len, sizeof(block->data.len), 1, f);
		fread(block->data.buffer, block->data.len, 1, f);
		fread(block->hash, SHA256_DIGEST_LENGTH, 1, f);
		fread(&nb_tx, sizeof(nb_tx), 1, f);
		if (nb_tx < 0)
			block->transactions = NULL;
		else
		{
			block->transactions = llist_create(MT_SUPPORT_FALSE);
			for (j = 0; j < (uint32_t)nb_tx; j++)
			{
				tx = read_transaction(f);
				if (tx)
					llist_add_node(block->transactions,
						tx, ADD_NODE_REAR);
			}
		}
		llist_add_node(bc->chain, block, ADD_NODE_REAR);
	}
	for (i = 0; i < nb_unspent; i++)
	{
		u = calloc(1, sizeof(*u));
		if (!u)
			break;
		fread(u->block_hash, SHA256_DIGEST_LENGTH, 1, f);
		fread(u->tx_id, SHA256_DIGEST_LENGTH, 1, f);
		fread(&u->out.amount, sizeof(u->out.amount), 1, f);
		fread(u->out.pub, EC_PUB_LEN, 1, f);
		fread(u->out.hash, SHA256_DIGEST_LENGTH, 1, f);
		llist_add_node(bc->unspent, u, ADD_NODE_REAR);
	}
	fclose(f);
	return (bc);
}
