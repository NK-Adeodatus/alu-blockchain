#include "hblk_crypto.h"

/**
 * _set_pub_key - Deserialises raw bytes into an EC_KEY's public key field
 *
 * @key: EC_KEY whose public key will be set
 * @pub: Buffer holding the uncompressed public key (EC_PUB_LEN bytes)
 *
 * Return: 1 on success, 0 on failure
 */
static int _set_pub_key(EC_KEY *key, uint8_t const pub[EC_PUB_LEN])
{
	EC_GROUP	*group;
	EC_POINT	*point;

	group = (EC_GROUP *)EC_KEY_get0_group(key);
	if (!group)
		return (0);

	point = EC_POINT_new(group);
	if (!point)
		return (0);

	if (!EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL))
	{
		EC_POINT_free(point);
		return (0);
	}

	if (!EC_KEY_set_public_key(key, point))
	{
		EC_POINT_free(point);
		return (0);
	}

	EC_POINT_free(point);
	return (1);
}

/**
 * ec_from_pub - Creates an EC_KEY structure from a public key
 *
 * @pub: Buffer holding the uncompressed public key (EC_PUB_LEN bytes)
 *
 * Return: Pointer to the created EC_KEY on success, or NULL on failure
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY	*key;

	if (!pub)
		return (NULL);

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	if (!_set_pub_key(key, pub))
	{
		EC_KEY_free(key);
		return (NULL);
	}

	return (key);
}
