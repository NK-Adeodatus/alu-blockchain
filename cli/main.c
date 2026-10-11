#include <stdio.h>
#include <string.h>
#include "blockchain.h"

void print_hash(uint8_t const *hash);
void print_block(block_t const *block);

/**
 * main - prints a saved blockchain as linked blocks
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char **argv)
{
	blockchain_t *chain;
	block_t *block;
	int i, size;

	if (argc != 3 || strcmp(argv[1], "view") != 0)
	{
		fprintf(stderr, "Usage: %s view <file>\n", argv[0]);
		return (1);
	}
	chain = blockchain_deserialize(argv[2]);
	if (!chain)
	{
		fprintf(stderr, "Failed to load %s\n", argv[2]);
		return (1);
	}
	size = llist_size(chain->chain);
	for (i = 0; i < size; i++)
	{
		block = llist_get_node_at(chain->chain, i);
		print_block(block);
		if (i < size - 1)
			printf("     |\n     v\n");
	}
	blockchain_destroy(chain);
	return (0);
}

/**
 * print_hash - prints the first 4 bytes of a hash in hex
 * @hash: the hash to print
 */
void print_hash(uint8_t const *hash)
{
	int i;

	for (i = 0; i < 4; i++)
		printf("%02x", hash[i]);
	printf("...");
}

/**
 * print_block - prints one block on a single line
 * @block: the block to print
 */
void print_block(block_t const *block)
{
	int tx = 0;

	if (block->transactions)
		tx = llist_size(block->transactions);
	printf("[Block %u] hash: ", block->info.index);
	print_hash(block->hash);
	printf("  prev: ");
	print_hash(block->info.prev_hash);
	printf("  (%d tx)\n", tx);
}
