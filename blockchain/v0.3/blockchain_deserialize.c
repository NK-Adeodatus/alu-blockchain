#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

/**
 * blockchain_deserialize - Deserializes a Blockchain from a file
 *
 * @path: Path to the file to deserialize from
 *
 * The function fails if the file cannot be opened, if the magic number
 * is wrong, or if the version does not match.
 *
 * Return: Pointer to the deserialized Blockchain, or NULL on failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE		*f;
	blockchain_t	*bc;
	block_t		*block;
	uint32_t	i, nb;
	uint8_t		hdr[8];

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
	if (fread(&nb, sizeof(nb), 1, f) != 1)
	{
		fclose(f);
		return (NULL);
	}
	bc = malloc(sizeof(*bc));
	bc->chain = llist_create(MT_SUPPORT_FALSE);
	for (i = 0; i < nb; i++)
	{
		block = calloc(1, sizeof(*block));
		fread(&block->info.index, sizeof(block->info.index), 1, f);
		fread(&block->info.difficulty, sizeof(block->info.difficulty), 1, f);
		fread(&block->info.timestamp, sizeof(block->info.timestamp), 1, f);
		fread(&block->info.nonce, sizeof(block->info.nonce), 1, f);
		fread(block->info.prev_hash, SHA256_DIGEST_LENGTH, 1, f);
		fread(&block->data.len, sizeof(block->data.len), 1, f);
		fread(block->data.buffer, block->data.len, 1, f);
		fread(block->hash, SHA256_DIGEST_LENGTH, 1, f);
		llist_add_node(bc->chain, block, ADD_NODE_REAR);
	}
	fclose(f);
	return (bc);
}
