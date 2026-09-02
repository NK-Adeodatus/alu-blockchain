#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "hblk_crypto.h"

/**
 * _write_priv_key - Writes the private key to <folder>/key.pem
 *
 * @key:    EC key pair whose private key will be written
 * @folder: Path to the destination folder
 *
 * Return: 1 on success, 0 on failure
 */
static int _write_priv_key(EC_KEY *key, char const *folder)
{
	char	path[512];
	FILE	*fp;
	int	ret;

	snprintf(path, sizeof(path), "%s/key.pem", folder);
	fp = fopen(path, "w");
	if (!fp)
		return (0);

	ret = PEM_write_ECPrivateKey(fp, key, NULL, NULL, 0, NULL, NULL);
	fclose(fp);
	return (ret ? 1 : 0);
}

/**
 * _write_pub_key - Writes the public key to <folder>/key_pub.pem
 *
 * @key:    EC key pair whose public key will be written
 * @folder: Path to the destination folder
 *
 * Return: 1 on success, 0 on failure
 */
static int _write_pub_key(EC_KEY *key, char const *folder)
{
	char		path[512];
	FILE		*fp;
	EVP_PKEY	*pkey;
	int		ret;

	snprintf(path, sizeof(path), "%s/key_pub.pem", folder);
	fp = fopen(path, "w");
	if (!fp)
		return (0);

	pkey = EVP_PKEY_new();
	if (!pkey)
	{
		fclose(fp);
		return (0);
	}

	EVP_PKEY_set1_EC_KEY(pkey, key);
	ret = PEM_write_PUBKEY(fp, pkey);
	EVP_PKEY_free(pkey);
	fclose(fp);
	return (ret ? 1 : 0);
}

/**
 * ec_save - Saves an EC key pair to disk in PEM format
 *
 * @key:    Pointer to the EC key pair to save
 * @folder: Path to the folder in which to save the keys
 *
 * Return: 1 on success, 0 on failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	if (!key || !folder)
		return (0);

	if (mkdir(folder, 0700) == -1 && errno != EEXIST)
		return (0);

	if (!_write_priv_key(key, folder))
		return (0);

	if (!_write_pub_key(key, folder))
		return (0);

	return (1);
}
