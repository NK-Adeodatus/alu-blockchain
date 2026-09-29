#include <stdio.h>
#include <stdint.h>

#include "blockchain.h"

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
	uint32_t	nb;
	int		i, size;
	block_t		*block;
	uint16_t	w = 0x0102;
	uint8_t		endian = *(uint8_t *)&w == 1 ? 2 : 1;

	f = fopen(path, "wb");
	if (!f)
		return (-1);
	size = llist_size(blockchain->chain);
	nb = (uint32_t)size;
	fwrite("HBLK", 1, 4, f);
	fwrite("0.1", 1, 3, f);
	fwrite(&endian, 1, 1, f);
	fwrite(&nb, sizeof(nb), 1, f);
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
	}
	fclose(f);
	return (0);
}
