#include <stdio.h>
#include <string.h>
#include <llist.h>

#include "../blockchain.h"

/**
 * _print_hex_buffer - Prints a buffer as a hexadecimal string
 *
 * @buf: Buffer to print
 * @len: Number of bytes to print
 */
static void _print_hex_buffer(uint8_t const *buf, size_t len)
{
	size_t	i;

	for (i = 0; i < len; i++)
		printf("%02x", buf[i]);
}

/**
 * _block_print - Prints a Block structure
 *
 * @block: Pointer to the Block to print
 * @index: Index parameter (unused — required by llist callback signature)
 * @indent: Indentation string
 */
static void _block_print(block_t const *block, unsigned int index,
	char const *indent)
{
	(void)index;
	printf("%sBlock: {\n", indent);

	printf("%s\tinfo: {\n", indent);
	printf("%s\t\tindex: %u,\n", indent, block->info.index);
	printf("%s\t\tdifficulty: %u,\n", indent, block->info.difficulty);
	printf("%s\t\ttimestamp: %lu,\n", indent, block->info.timestamp);
	printf("%s\t\tnonce: %lu,\n", indent, block->info.nonce);
	printf("%s\t\tprev_hash: ", indent);
	_print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
	printf("\n");
	printf("%s\t},\n", indent);

	printf("%s\tdata: {\n", indent);
	printf("%s\t\tbuffer: \"%s\",\n", indent, block->data.buffer);
	printf("%s\t\tlen: %u\n", indent, block->data.len);
	printf("%s\t},\n", indent);

	printf("%s\thash: ", indent);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
	printf("\n");

	printf("%s}\n", indent);
}

/**
 * _blockchain_print - Prints a Blockchain structure
 *
 * @blockchain: Pointer to the Blockchain to print
 */
void _blockchain_print(blockchain_t const *blockchain)
{
	int	size;

	size = llist_size(blockchain->chain);

	printf("Blockchain: {\n");
	printf("\tchain [%d]: [\n", size);
	llist_for_each(blockchain->chain,
		(node_func_t)_block_print, "\t\t");
	printf("\t]\n");
	printf("}\n");
}
