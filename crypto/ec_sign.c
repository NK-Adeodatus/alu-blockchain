#include "hblk_crypto.h"

/**
 * ec_sign - Signs a sequence of bytes using an EC private key
 *
 * @key:    EC_KEY containing the private key used to sign
 * @msg:    Pointer to the sequence of bytes to sign
 * @msglen: Number of bytes in @msg
 * @sig:    Address at which to store the resulting signature
 *
 * Return: Pointer to sig->sig on success, or NULL on failure
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg,
		size_t msglen, sig_t *sig)
{
	unsigned int siglen;

	if (!key || !msg)
		return (NULL);

	if (!ECDSA_sign(0, msg, (int)msglen, sig->sig,
			&siglen, (EC_KEY *)key))
		return (NULL);

	sig->len = (uint8_t)siglen;
	return (sig->sig);
}
