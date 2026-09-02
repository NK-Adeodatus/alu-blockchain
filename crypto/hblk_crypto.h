#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H

#include <stdint.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

/* --- SHA256 --- */

/**
 * SHA256_DIGEST_LENGTH - Length of a SHA-256 digest in bytes
 */
#ifndef SHA256_DIGEST_LENGTH
#define SHA256_DIGEST_LENGTH 32
#endif

uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH]);

/* --- EC key pair --- */

/**
 * EC_CURVE - Elliptic curve used for key generation
 */
#define EC_CURVE NID_secp256k1

/**
 * EC_PUB_LEN - Length of a serialised uncompressed EC public key (bytes)
 */
#define EC_PUB_LEN 65

/**
 * PRI_FILENAME - Name of the file storing the EC private key
 */
#define PRI_FILENAME "key.pem"

/**
 * PUB_FILENAME - Name of the file storing the EC public key
 */
#define PUB_FILENAME "key_pub.pem"

/**
 * struct ec_key_s - Wrapper around an OpenSSL EC_KEY
 * @key: Pointer to the wrapped EC key
 */
typedef struct ec_key_s
{
	EC_KEY	*key;
} ec_key_t;

/**
 * struct sig_s - Elliptic-curve signature
 * @sig: DER-encoded signature bytes
 * @len: Number of valid bytes in @sig
 */
typedef struct sig_s
{
	uint8_t	sig[72];
	uint8_t	len;
} sig_t;

EC_KEY		*ec_create(void);
uint8_t		*ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);
EC_KEY		*ec_from_pub(uint8_t const pub[EC_PUB_LEN]);
int		ec_save(EC_KEY *key, char const *folder);
EC_KEY		*ec_load(char const *folder);
int		ec_sign(EC_KEY const *key, uint8_t const *msg,
			size_t msglen, sig_t *sig);
int		ec_verify(EC_KEY const *key, uint8_t const *msg,
			size_t msglen, sig_t const *sig);

#endif /* HBLK_CRYPTO_H */
