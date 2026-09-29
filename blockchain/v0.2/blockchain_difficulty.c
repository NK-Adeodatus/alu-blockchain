#include "blockchain.h"

/**
 * blockchain_difficulty - computes the difficulty of the next Block
 * @blockchain: pointer to the Blockchain to analyze
 *
 * Return: difficulty to assign to the next Block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *last, *adjusted;
	uint64_t expected, actual;

	last = llist_get_tail(blockchain->chain);
	if (last->info.index == 0 ||
	    last->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL != 0)
		return (last->info.difficulty);

	adjusted = llist_get_node_at(blockchain->chain,
		llist_size(blockchain->chain) - DIFFICULTY_ADJUSTMENT_INTERVAL);
	expected = BLOCK_GENERATION_INTERVAL * DIFFICULTY_ADJUSTMENT_INTERVAL;
	actual = last->info.timestamp - adjusted->info.timestamp;

	if (actual * 2 < expected)
		return (last->info.difficulty + 1);
	if (actual > expected * 2 && last->info.difficulty > 0)
		return (last->info.difficulty - 1);
	return (last->info.difficulty);
}
