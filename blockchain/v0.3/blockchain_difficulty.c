#include "blockchain.h"

/**
 * blockchain_difficulty - Computes the difficulty for the next block
 *
 * @blockchain: Pointer to the Blockchain to compute the difficulty of
 *
 * Return: The difficulty for the next Block to mine
 *
 * Rules:
 * - If the index of the last Block in @blockchain is a multiple of
 *   DIFFICULTY_ADJUSTMENT_INTERVAL, the difficulty must be adjusted.
 * - Otherwise, the difficulty of the last Block in @blockchain is used.
 * - The difficulty is adjusted by examining the average time between
 *   the creation of the last DIFFICULTY_ADJUSTMENT_INTERVAL Blocks.
 * - If the average time is below BLOCK_GENERATION_INTERVAL, increase
 *   difficulty by 1.
 * - Otherwise, decrease it by 1 (but never below 0).
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *last;
	block_t *adj;
	int size;
	uint64_t elapsed;
	uint32_t difficulty;

	if (!blockchain)
		return (0);

	size = llist_size(blockchain->chain);
	last = llist_get_node_at(blockchain->chain, size - 1);
	difficulty = last->info.difficulty;

	if (last->info.index == 0)
		return (DIFFICULTY_ADJUSTMENT_INTERVAL > 0 ?
			1 : difficulty);

	if (last->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL != 0)
		return (difficulty);

	adj = llist_get_node_at(blockchain->chain,
		size - 1 - DIFFICULTY_ADJUSTMENT_INTERVAL);
	elapsed = last->info.timestamp - adj->info.timestamp;

	if (elapsed < (uint64_t)BLOCK_GENERATION_INTERVAL *
		DIFFICULTY_ADJUSTMENT_INTERVAL)
		return (difficulty + 1);

	if (difficulty > 0)
		return (difficulty - 1);
	return (0);
}
