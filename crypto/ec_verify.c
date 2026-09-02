#include "hblk_crypto.h"

/**
 * ec_verify - Verifies the signature of a sequence of bytes
 *
 * @key:    EC_KEY containing the public key used to verify the signature
 * @msg:    Pointer to the sequence of bytes whose signature is to be checked
 * @msglen: Number of bytes in @msg
 * @sig:    Pointer to the signature to verify
 *
 * Return: 1 if the signature is valid, 0 otherwise
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg,
		size_t msglen, sig_t const *sig)
{
	if (!key || !msg || !sig)
		return (0);

	return (ECDSA_verify(0, msg, (int)msglen,
		sig->sig, (int)sig->len, (EC_KEY *)key) == 1);
}
