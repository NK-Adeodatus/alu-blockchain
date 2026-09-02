#include "hblk_crypto.h"

/**
 * ec_from_pub - Creates an EC_KEY structure from a public key
 *
 * @pub: Buffer holding the uncompressed public key (EC_PUB_LEN bytes)
 *
 * Return: Pointer to the created EC_KEY on success, or NULL on failure
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY		*key;
	EC_GROUP	*group;
	EC_POINT	*point;

	if (!pub)
		return (NULL);

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	group = (EC_GROUP *)EC_KEY_get0_group(key);
	if (!group)
	{
		EC_KEY_free(key);
		return (NULL);
	}

	point = EC_POINT_new(group);
	if (!point)
	{
		EC_KEY_free(key);
		return (NULL);
	}

	if (!EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL))
	{
		EC_POINT_free(point);
		EC_KEY_free(key);
		return (NULL);
	}

	if (!EC_KEY_set_public_key(key, point))
	{
		EC_POINT_free(point);
		EC_KEY_free(key);
		return (NULL);
	}

	EC_POINT_free(point);
	return (key);
}
