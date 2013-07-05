/*
 * pbkdf2.c
 *
 *  Created on: Jul 5, 2013
 *      Author: andy
 */

#include <tropicssl/pbkdf2.h>

int pbkdf2_sha1_hmac(const unsigned char *password, unsigned int plen,
		const unsigned char *salt, unsigned int slen, unsigned int iteration_count,
		unsigned int key_length, unsigned char *output) {
	int j;
	unsigned int i;
	unsigned char u_c_1[20];
	unsigned char u_c[20];
	unsigned char digest_len = 20;
	unsigned int use_len;
	unsigned char *out_p = output;
	unsigned char counter[4];
	sha1_context ctx;

	memset(counter, 0, 4);
	counter[3] = 1;

	if (iteration_count > 0xFFFFFFFF)
		return (-1);

	while (key_length) {
		// U1 ends up in work
		//
		sha1_starts(&ctx);
		sha1_update(&ctx, password, plen);
		sha1_update(&ctx, salt, slen);
		sha1_update(&ctx, counter, 4);
		sha1_finish(&ctx, u_c);

		memcpy(u_c_1, u_c, digest_len);

		for (i = 1; i < iteration_count; i++) {
			sha1_starts(&ctx);
			sha1_update(&ctx, password, plen);
			sha1_update(&ctx, u_c_1, digest_len);
			sha1_finish(&ctx, u_c_1);

			for (j = 0; j < digest_len; j++)
				u_c[j] ^= u_c_1[j];
		}

		use_len = (key_length < digest_len) ? key_length : digest_len;
		memcpy(out_p, u_c, use_len);

		key_length -= use_len;
		out_p += use_len;

		for (i = 4; i > 0; i--)
			if (++counter[i - 1] != 0)
				break;
	}

	return (0);
}

int pbkdf2_sha256_hmac(const unsigned char *password, unsigned int plen,
		const unsigned char *salt, unsigned int slen, unsigned int iteration_count,
		int key_length, unsigned char *output) {
	int j;
	unsigned int i;
	unsigned char u_c_1[32];
	unsigned char u_c[32];
	unsigned char digest_len = 32;
	unsigned int use_len;
	unsigned char *out_p = output;
	unsigned char counter[4];
	sha2_context ctx;

	memset(counter, 0, 4);
	counter[3] = 1;

	if (iteration_count > 0xFFFFFFFF)
		return (-1);

	while (key_length) {
		// U1 ends up in work
		//
		sha2_starts(&ctx, 0);
		sha2_update(&ctx, password, plen);
		sha2_update(&ctx, salt, slen);
		sha2_update(&ctx, counter, 4);
		sha2_finish(&ctx, u_c);

		memcpy(u_c_1, u_c, digest_len);

		for (i = 1; i < iteration_count; i++) {
			sha2_starts(&ctx, 0);
			sha2_update(&ctx, password, plen);
			sha2_update(&ctx, u_c_1, digest_len);
			sha2_finish(&ctx, u_c_1);

			for (j = 0; j < digest_len; j++)
				u_c[j] ^= u_c_1[j];
		}

		use_len = (key_length < digest_len) ? key_length : digest_len;
		memcpy(out_p, u_c, use_len);

		key_length -= use_len;
		out_p += use_len;

		for (i = 4; i > 0; i--)
			if (++counter[i - 1] != 0)
				break;
	}

	return (0);
}

int pbkdf2_sha224_hmac(const unsigned char *password, unsigned int plen,
		const unsigned char *salt, unsigned int slen, unsigned int iteration_count,
		unsigned int key_length, unsigned char *output) {
	int j;
	unsigned int i;
	unsigned char u_c_1[28];
	unsigned char u_c[28];
	unsigned char digest_len = 28;
	unsigned int use_len;
	unsigned char *out_p = output;
	unsigned char counter[4];
	sha2_context ctx;

	memset(counter, 0, 4);
	counter[3] = 1;

	if (iteration_count > 0xFFFFFFFF)
		return (-1);

	while (key_length) {
		// U1 ends up in work
		//
		sha2_starts(&ctx, 1);
		sha2_update(&ctx, password, plen);
		sha2_update(&ctx, salt, slen);
		sha2_update(&ctx, counter, 4);
		sha2_finish(&ctx, u_c);

		memcpy(u_c_1, u_c, digest_len);

		for (i = 1; i < iteration_count; i++) {
			sha2_starts(&ctx, 1);
			sha2_update(&ctx, password, plen);
			sha2_update(&ctx, u_c_1, digest_len);
			sha2_finish(&ctx, u_c_1);

			for (j = 0; j < digest_len; j++)
				u_c[j] ^= u_c_1[j];
		}

		use_len = (key_length < digest_len) ? key_length : digest_len;
		memcpy(out_p, u_c, use_len);

		key_length -= use_len;
		out_p += use_len;

		for (i = 4; i > 0; i--)
			if (++counter[i - 1] != 0)
				break;
	}

	return (0);
}
