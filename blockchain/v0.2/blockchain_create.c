#include <stdlib.h>
#include <string.h>

#include "blockchain.h"

/**
 * blockchain_create - Creates a Blockchain and initialises it
 *
 * The Blockchain is initialised with one statically pre-defined Genesis Block.
 *
 * Return: Pointer to the newly allocated Blockchain, or NULL on failure
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t	*blockchain;
	block_t		*genesis;

	blockchain = malloc(sizeof(*blockchain));
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}
	genesis = calloc(1, sizeof(*genesis));
	if (!genesis)
	{
		llist_destroy(blockchain->chain, 0, NULL);
		free(blockchain);
		return (NULL);
	}
	genesis->info.index = GENESIS_INDEX;
	genesis->info.difficulty = GENESIS_DIFFICULTY;
	genesis->info.timestamp = GENESIS_TIMESTAMP;
	genesis->info.nonce = GENESIS_NONCE;
	memcpy(genesis->data.buffer, GENESIS_DATA, GENESIS_DATA_LEN);
	genesis->data.len = GENESIS_DATA_LEN;
	memcpy(genesis->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH);
	if (llist_add_node(blockchain->chain, genesis, ADD_NODE_REAR) != 0)
	{
		free(genesis);
		llist_destroy(blockchain->chain, 0, NULL);
		free(blockchain);
		return (NULL);
	}
	return (blockchain);
}
