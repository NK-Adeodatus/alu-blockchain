#include <stdio.h>
#include "hblk_crypto.h"

/**
 * ec_load - Loads an EC key pair from PEM files on disk
 *
 * @folder: Path to the folder containing the key files
 *
 * Return: Pointer to the loaded EC_KEY on success, or NULL on failure
 */
EC_KEY *ec_load(char const *folder)
{
	char	path[512];
	FILE	*fp;
	EC_KEY	*key;

	if (!folder)
		return (NULL);

	snprintf(path, sizeof(path), "%s/" PRI_FILENAME, folder);

	fp = fopen(path, "r");
	if (!fp)
		return (NULL);

	key = PEM_read_ECPrivateKey(fp, NULL, NULL, NULL);
	fclose(fp);

	return (key);
}
