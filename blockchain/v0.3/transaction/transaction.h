#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../crypto/hblk_crypto.h"

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

/* --- tx_out_create --- */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

#endif /* TRANSACTION_H */