#include "hblk_crypto.h"

/**
 * ec_to_pub - Extracts the public key from an EC_KEY structure
 *
 * @key: Pointer to the EC_KEY to extract the public key from
 * @pub: Buffer in which to store the extracted public key (uncompressed)
 *
 * Return: Pointer to @pub on success, or NULL on failure
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_POINT	*pub_point;
	const EC_GROUP	*group;
	size_t		len;

	if (!key)
		return (NULL);

	pub_point = EC_KEY_get0_public_key(key);
	if (!pub_point)
		return (NULL);

	group = EC_KEY_get0_group(key);
	if (!group)
		return (NULL);

	len = EC_POINT_point2oct(group, pub_point,
		POINT_CONVERSION_UNCOMPRESSED, pub, EC_PUB_LEN, NULL);

	if (len != EC_PUB_LEN)
		return (NULL);

	return (pub);
}
