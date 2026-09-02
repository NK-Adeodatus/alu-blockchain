#include "hblk_crypto.h"

/**
 * ec_create - Creates a new EC key pair on the secp256k1 curve
 *
 * Return: Pointer to the generated EC_KEY structure containing both the
 *         private and public keys, or NULL on failure
 */
EC_KEY *ec_create(void)
{
	EC_KEY *key;

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	if (!EC_KEY_generate_key(key))
	{
		EC_KEY_free(key);
		return (NULL);
	}

	return (key);
}
