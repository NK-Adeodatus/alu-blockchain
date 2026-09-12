#include <stdlib.h>
#include <llist.h>

#include "../blockchain.h"

/**
 * _block_delete - Frees a Block
 *
 * @block:  Pointer to the Block to free
 * @index:  Index of the Block in the list (unused)
 * @unused: Unused argument required by llist callback signature
 */
static void _block_delete(block_t *block, unsigned int index, void *unused)
{
	(void)index;
	(void)unused;
	free(block);
}

/**
 * _blockchain_destroy - Frees a Blockchain and all its Blocks
 *
 * @blockchain: Pointer to the Blockchain to free
 */
void _blockchain_destroy(blockchain_t *blockchain)
{
	llist_destroy(blockchain->chain, 1, (node_dtor_t)_block_delete);
	free(blockchain);
}
