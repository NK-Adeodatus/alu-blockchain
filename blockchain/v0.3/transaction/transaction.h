#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../crypto/hblk_crypto.h"
#include <llist.h>

/**
 * struct tx_out_s - Transaction output
 *
 * @amount: Amount received
 * @pub:    Receiver's public address
 * @hash:   Hash of @amount and @pub
 */
typedef struct tx_out_s
{
	uint32_t	amount;
	uint8_t		pub[EC_PUB_LEN];
	uint8_t		hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/**
 * struct tx_in_s - Transaction input
 *
 * @block_hash: Hash of the Block containing the transaction @tx_out_hash
 * @tx_id:      ID of the transaction containing @tx_out_hash
 * @tx_out_hash: Hash of the referenced transaction output
 * @sig:        Signature. Prevents anyone from altering the content of the
 *              transaction. The signature is computed on the transaction ID
 *              and the public key of the receiver
 */
typedef struct tx_in_s
{
	uint8_t		block_hash[SHA256_DIGEST_LENGTH];
	uint8_t		tx_id[SHA256_DIGEST_LENGTH];
	uint8_t		tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t		sig;
} tx_in_t;

/**
 * struct unspent_tx_out_s - Unspent transaction output
 *
 * @block_hash: Hash of the Block containing the transaction @tx_out_hash
 * @tx_id:      ID of the transaction containing @tx_out_hash
 * @out:        Copy of the referenced transaction output
 */
typedef struct unspent_tx_out_s
{
	uint8_t		block_hash[SHA256_DIGEST_LENGTH];
	uint8_t		tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t	out;
} unspent_tx_out_t;

/**
 * struct transaction_s - Transaction structure
 *
 * @id:      Transaction ID. A hash of all the inputs and outputs.
 *           Prevents further alteration of the transaction.
 * @inputs:  List of `tx_in_t *`. Transaction inputs
 * @outputs: List of `tx_out_t *`. Transaction outputs
 */
typedef struct transaction_s
{
	uint8_t		id[SHA256_DIGEST_LENGTH];
	llist_t		*inputs;
	llist_t		*outputs;
} transaction_t;

/**
 * struct find_unspent_s - Helper to find an unspent transaction output
 *
 * @block_hash:  Block hash to match
 * @tx_id:       Transaction ID to match
 * @tx_out_hash: Hash to search for
 * @found:       Pointer to the matching unspent output if found
 */
typedef struct find_unspent_s
{
	uint8_t const		*block_hash;
	uint8_t const		*tx_id;
	uint8_t const		*tx_out_hash;
	unspent_tx_out_t	*found;
} find_unspent_t;

/**
 * SIG_MAX_LEN - Maximum length of a DER-encoded signature
 */
#define SIG_MAX_LEN 72

/**
 * COINBASE_AMOUNT - Amount of coins awarded in a coinbase transaction
 */
#define COINBASE_AMOUNT 50

/**
 * struct collect_tx_s - Context for collecting matching unspent outputs
 *
 * @sender_pub: Public key of the sender
 * @inputs:     List of transaction inputs being built
 * @total:      Running total of collected amounts
 * @amount:     Target amount to collect
 * @done:       Set to 1 once enough inputs are collected
 */
typedef struct collect_tx_s
{
	uint8_t		sender_pub[EC_PUB_LEN];
	llist_t		*inputs;
	uint32_t	total;
	uint32_t	amount;
	int		done;
} collect_tx_t;

/**
 * struct sign_ctx_s - Context for signing transaction inputs
 *
 * @tx_id:       ID of the transaction
 * @sender:      Sender's private key
 * @all_unspent: List of all unspent transaction outputs
 * @valid:       Set to 0 if signing fails
 */
typedef struct sign_ctx_s
{
	uint8_t const	*tx_id;
	EC_KEY const	*sender;
	llist_t		*all_unspent;
	int		valid;
} sign_ctx_t;

/**
 * struct valid_in_ctx_s - Context for validating transaction inputs
 *
 * @all_unspent: List of all unspent transaction outputs
 * @tx_id:       Expected transaction ID
 * @in_total:    Running total of input amounts
 * @valid:       Set to 0 if any input is invalid
 */
typedef struct valid_in_ctx_s
{
	llist_t		*all_unspent;
	uint8_t const	*tx_id;
	uint32_t	in_total;
	int		valid;
} valid_in_ctx_t;

/* --- tx_out_create --- */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

/* --- unspent_tx_out_create --- */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out);

/* --- tx_in_create --- */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);

/* --- transaction_hash --- */
uint8_t *transaction_hash(transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

/* --- tx_in_sign --- */
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender, llist_t *all_unspent);

/* --- transaction_create --- */
transaction_t *transaction_create(EC_KEY const *sender,
	EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent);

/* --- transaction_is_valid --- */
int transaction_is_valid(transaction_t const *transaction,
	llist_t *all_unspent);

/* --- coinbase_create --- */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index);

/* --- coinbase_is_valid --- */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);


/**
 * struct add_ctx_s - Context for adding unspent outputs from transactions
 *
 * @block_hash: Hash of the block containing the transactions
 * @new_list:   New list of unspent transaction outputs
 */
typedef struct add_ctx_s
{
	uint8_t		*block_hash;
	llist_t		*new_list;
} add_ctx_t;

/**
 * struct filter_ctx_s - Context for filtering spent outputs
 *
 * @transactions: List of processed transactions
 * @new_list:     New list to copy unspent outputs to
 */
typedef struct filter_ctx_s
{
	llist_t	*transactions;
	llist_t	*new_list;
} filter_ctx_t;

/* --- transaction_destroy --- */
void transaction_destroy(transaction_t *transaction);
llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent);

#endif /* TRANSACTION_H */
