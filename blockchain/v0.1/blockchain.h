#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stdlib.h>
#include <llist.h>

#include "../../crypto/hblk_crypto.h"

/* --- Macros --- */

/**
 * BLOCKCHAIN_DATA_MAX - Maximum size of data stored in a block
 */
#define BLOCKCHAIN_DATA_MAX 1024

/**
 * Genesis Block constants
 */
#define GENESIS_INDEX      0
#define GENESIS_DIFFICULTY 0
#define GENESIS_TIMESTAMP  1537578000
#define GENESIS_NONCE      0
#define GENESIS_DATA       "Holberton School"
#define GENESIS_DATA_LEN   16
#define GENESIS_HASH \
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"

/* --- Data structures --- */

/**
 * struct block_info_s - Information stored in a Block
 *
 * @index:      Rank of the Block in the Blockchain
 * @difficulty: Difficulty of the proof of work needed to mine this Block
 * @timestamp:  Time at which the block was created (UNIX timestamp)
 * @nonce:      Salt used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
	uint32_t	index;
	uint32_t	difficulty;
	uint64_t	timestamp;
	uint64_t	nonce;
	uint8_t		prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_data_s - Data stored in a Block
 *
 * @buffer: Data buffer
 * @len:    Length of the data stored in @buffer (in bytes)
 */
typedef struct block_data_s
{
	int8_t		buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t	len;
} block_data_t;

/**
 * struct block_s - Block in the Blockchain
 *
 * @info: Block metadata
 * @data: Block data
 * @hash: SHA256 hash of the Block (based on its @info and @data)
 */
typedef struct block_s
{
	block_info_t	info;
	block_data_t	data;
	uint8_t		hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_s - Blockchain structure
 *
 * @chain: Linked list of pointers to each Block in the Blockchain
 */
typedef struct blockchain_s
{
	llist_t		*chain;
} blockchain_t;

/* --- Prototypes --- */

blockchain_t	*blockchain_create(void);
block_t		*block_create(block_t const *prev, int8_t const *data,
			uint32_t data_len);
void		block_destroy(block_t *block);
void		blockchain_destroy(blockchain_t *blockchain);
uint8_t		*block_hash(block_t const *block,
			uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int		blockchain_serialize(blockchain_t const *blockchain,
			char const *path);
blockchain_t	*blockchain_deserialize(char const *path);
int		block_is_valid(block_t const *block,
			block_t const *prev_block);

#endif /* BLOCKCHAIN_H */
