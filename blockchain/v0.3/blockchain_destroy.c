#include <stdlib.h>
#include <llist.h>

#include "blockchain.h"

/**
 * blockchain_destroy - Frees a Blockchain and all its Blocks
 *
 * @blockchain: Pointer to the Blockchain to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	llist_destroy(blockchain->chain, 1, (node_dtor_t)block_destroy);
	llist_destroy(blockchain->unspent, 1, free);
	free(blockchain);
}
