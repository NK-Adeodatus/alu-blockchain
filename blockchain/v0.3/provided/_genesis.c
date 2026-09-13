#include <string.h>

#include "../blockchain.h"

/**
 * _get_genesis_block - Returns a pointer to the static genesis block data
 *
 * Return: Pointer to a statically allocated block_t containing genesis data
 */
block_t const *_get_genesis_block(void)
{
	static block_t genesis;
	static int initialised;

	if (!initialised)
	{
		memset(&genesis, 0, sizeof(genesis));
		genesis.info.index      = GENESIS_INDEX;
		genesis.info.difficulty = GENESIS_DIFFICULTY;
		genesis.info.timestamp  = GENESIS_TIMESTAMP;
		genesis.info.nonce      = GENESIS_NONCE;
		/* prev_hash: all zeros (already zeroed by memset) */
		memcpy(genesis.data.buffer, GENESIS_DATA, GENESIS_DATA_LEN);
		genesis.data.len = GENESIS_DATA_LEN;
		memcpy(genesis.hash, GENESIS_HASH, SHA256_DIGEST_LENGTH);
		initialised = 1;
	}
	return (&genesis);
}
