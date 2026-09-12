#include <stdlib.h>

#include "blockchain.h"

/**
 * block_destroy - Frees a Block structure
 *
 * @block: Pointer to the Block to delete
 */
void block_destroy(block_t *block)
{
	free(block);
}
