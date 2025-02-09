/*
 *	RFC 1186/1320 compliant MD4 implementation
 *
 *	Based on XySSL: Copyright (C) 2006-2008	 Christophe Devine
 *
 *	Copyright (C) 2009	Paul Bakker <polarssl_maintainer at polarssl dot org>
 *
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions
 *	are met:
 *
 *	  * Redistributions of source code must retain the above copyright
 *		notice, this list of conditions and the following disclaimer.
 *	  * Redistributions in binary form must reproduce the above copyright
 *		notice, this list of conditions and the following disclaimer in the
 *		documentation and/or other materials provided with the distribution.
 *	  * Neither the names of PolarSSL or XySSL nor the names of its contributors
 *		may be used to endorse or promote products derived from this software
 *		without specific prior written permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *	OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *	The MD4 algorithm was designed by Ron Rivest in 1990.
 *
 *	http://www.ietf.org/rfc/rfc1186.txt
 *	http://www.ietf.org/rfc/rfc1320.txt
 */
/**
 * changed by Andreas Fitzek to compile in ANDIX kernelspace 2013
 */
#include <tropicssl/config.h>

#if defined(TROPICSSL_MD4_C)

#include <tropicssl/md4.h>

/*
 * 32-bit integer manipulation macros (little endian)
 */
#ifndef GET_ULONG_LE
#define GET_ULONG_LE(n,b,i)								\
	{													\
		(n) = ( (unsigned long) (b)[(i)	   ]	   )	\
			| ( (unsigned long) (b)[(i) + 1] <<	 8 )	\
			| ( (unsigned long) (b)[(i) + 2] << 16 )	\
			| ( (unsigned long) (b)[(i) + 3] << 24 );	\
	}
#endif

#ifndef PUT_ULONG_LE
#define PUT_ULONG_LE(n,b,i)								\
	{													\
		(b)[(i)	   ] = (unsigned char) ( (n)	   );	\
		(b)[(i) + 1] = (unsigned char) ( (n) >>	 8 );	\
		(b)[(i) + 2] = (unsigned char) ( (n) >> 16 );	\
		(b)[(i) + 3] = (unsigned char) ( (n) >> 24 );	\
	}
#endif

/*
 * MD4 context setup
 */
void md4_starts(md4_context * ctx)
{
	ctx->total[0] = 0;
	ctx->total[1] = 0;

	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
}

static void md4_process(md4_context * ctx, unsigned char data[64])
{
	unsigned long X[16], A, B, C, D;

	GET_ULONG_LE(X[0], data, 0);
	GET_ULONG_LE(X[1], data, 4);
	GET_ULONG_LE(X[2], data, 8);
	GET_ULONG_LE(X[3], data, 12);
	GET_ULONG_LE(X[4], data, 16);
	GET_ULONG_LE(X[5], data, 20);
	GET_ULONG_LE(X[6], data, 24);
	GET_ULONG_LE(X[7], data, 28);
	GET_ULONG_LE(X[8], data, 32);
	GET_ULONG_LE(X[9], data, 36);
	GET_ULONG_LE(X[10], data, 40);
	GET_ULONG_LE(X[11], data, 44);
	GET_ULONG_LE(X[12], data, 48);
	GET_ULONG_LE(X[13], data, 52);
	GET_ULONG_LE(X[14], data, 56);
	GET_ULONG_LE(X[15], data, 60);

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];

#define F(x, y, z) ((x & y) | ((~x) & z))
#define P(a,b,c,d,x,s) { a += F(b,c,d) + x; a = S(a,s); }

	P(A, B, C, D, X[0], 3);
	P(D, A, B, C, X[1], 7);
	P(C, D, A, B, X[2], 11);
	P(B, C, D, A, X[3], 19);
	P(A, B, C, D, X[4], 3);
	P(D, A, B, C, X[5], 7);
	P(C, D, A, B, X[6], 11);
	P(B, C, D, A, X[7], 19);
	P(A, B, C, D, X[8], 3);
	P(D, A, B, C, X[9], 7);
	P(C, D, A, B, X[10], 11);
	P(B, C, D, A, X[11], 19);
	P(A, B, C, D, X[12], 3);
	P(D, A, B, C, X[13], 7);
	P(C, D, A, B, X[14], 11);
	P(B, C, D, A, X[15], 19);

#undef P
#undef F

#define F(x,y,z) ((x & y) | (x & z) | (y & z))
#define P(a,b,c,d,x,s) { a += F(b,c,d) + x + 0x5A827999; a = S(a,s); }

	P(A, B, C, D, X[0], 3);
	P(D, A, B, C, X[4], 5);
	P(C, D, A, B, X[8], 9);
	P(B, C, D, A, X[12], 13);
	P(A, B, C, D, X[1], 3);
	P(D, A, B, C, X[5], 5);
	P(C, D, A, B, X[9], 9);
	P(B, C, D, A, X[13], 13);
	P(A, B, C, D, X[2], 3);
	P(D, A, B, C, X[6], 5);
	P(C, D, A, B, X[10], 9);
	P(B, C, D, A, X[14], 13);
	P(A, B, C, D, X[3], 3);
	P(D, A, B, C, X[7], 5);
	P(C, D, A, B, X[11], 9);
	P(B, C, D, A, X[15], 13);

#undef P
#undef F

#define F(x,y,z) (x ^ y ^ z)
#define P(a,b,c,d,x,s) { a += F(b,c,d) + x + 0x6ED9EBA1; a = S(a,s); }

	P(A, B, C, D, X[0], 3);
	P(D, A, B, C, X[8], 9);
	P(C, D, A, B, X[4], 11);
	P(B, C, D, A, X[12], 15);
	P(A, B, C, D, X[2], 3);
	P(D, A, B, C, X[10], 9);
	P(C, D, A, B, X[6], 11);
	P(B, C, D, A, X[14], 15);
	P(A, B, C, D, X[1], 3);
	P(D, A, B, C, X[9], 9);
	P(C, D, A, B, X[5], 11);
	P(B, C, D, A, X[13], 15);
	P(A, B, C, D, X[3], 3);
	P(D, A, B, C, X[11], 9);
	P(C, D, A, B, X[7], 11);
	P(B, C, D, A, X[15], 15);

#undef F
#undef P

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
}

/*
 * MD4 process buffer
 */
void md4_update(md4_context * ctx, unsigned char *input, int ilen)
{
	int fill;
	unsigned long left;

	if (ilen <= 0)
		return;

	left = ctx->total[0] & 0x3F;
	fill = 64 - left;

	ctx->total[0] += ilen;
	ctx->total[0] &= 0xFFFFFFFF;

	if (ctx->total[0] < (unsigned long)ilen)
		ctx->total[1]++;

	if (left && ilen >= fill) {
		memcpy((void *)(ctx->buffer + left), (void *)input, fill);
		md4_process(ctx, ctx->buffer);
		input += fill;
		ilen -= fill;
		left = 0;
	}

	while (ilen >= 64) {
		md4_process(ctx, input);
		input += 64;
		ilen -= 64;
	}

	if (ilen > 0) {
		memcpy((void *)(ctx->buffer + left), (void *)input, ilen);
	}
}

static const unsigned char md4_padding[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
 * MD4 final digest
 */
void md4_finish(md4_context * ctx, unsigned char output[16])
{
	unsigned long last, padn;
	unsigned long high, low;
	unsigned char msglen[8];

	high = (ctx->total[0] >> 29)
	    | (ctx->total[1] << 3);
	low = (ctx->total[0] << 3);

	PUT_ULONG_LE(low, msglen, 0);
	PUT_ULONG_LE(high, msglen, 4);

	last = ctx->total[0] & 0x3F;
	padn = (last < 56) ? (56 - last) : (120 - last);

	md4_update(ctx, (unsigned char *)md4_padding, padn);
	md4_update(ctx, msglen, 8);

	PUT_ULONG_LE(ctx->state[0], output, 0);
	PUT_ULONG_LE(ctx->state[1], output, 4);
	PUT_ULONG_LE(ctx->state[2], output, 8);
	PUT_ULONG_LE(ctx->state[3], output, 12);
}

/*
 * output = MD4( input buffer )
 */
void md4(unsigned char *input, int ilen, unsigned char output[16])
{
	md4_context ctx;

	md4_starts(&ctx);
	md4_update(&ctx, input, ilen);
	md4_finish(&ctx, output);

	memset(&ctx, 0, sizeof(md4_context));
}

/*
 * MD4 HMAC context setup
 */
void md4_hmac_starts(md4_context * ctx, unsigned char *key, int keylen)
{
	int i;
	unsigned char sum[16];

	if (keylen > 64) {
		md4(key, keylen, sum);
		keylen = 16;
		key = sum;
	}

	memset(ctx->ipad, 0x36, 64);
	memset(ctx->opad, 0x5C, 64);

	for (i = 0; i < keylen; i++) {
		ctx->ipad[i] = (unsigned char)(ctx->ipad[i] ^ key[i]);
		ctx->opad[i] = (unsigned char)(ctx->opad[i] ^ key[i]);
	}

	md4_starts(ctx);
	md4_update(ctx, ctx->ipad, 64);

	memset(sum, 0, sizeof(sum));
}

/*
 * MD4 HMAC process buffer
 */
void md4_hmac_update(md4_context * ctx, unsigned char *input, int ilen)
{
	md4_update(ctx, input, ilen);
}

/*
 * MD4 HMAC final digest
 */
void md4_hmac_finish(md4_context * ctx, unsigned char output[16])
{
	unsigned char tmpbuf[16];

	md4_finish(ctx, tmpbuf);
	md4_starts(ctx);
	md4_update(ctx, ctx->opad, 64);
	md4_update(ctx, tmpbuf, 16);
	md4_finish(ctx, output);

	memset(tmpbuf, 0, sizeof(tmpbuf));
}

/*
 * output = HMAC-MD4( hmac key, input buffer )
 */
void md4_hmac(unsigned char *key, int keylen, unsigned char *input, int ilen,
	      unsigned char output[16])
{
	md4_context ctx;

	md4_hmac_starts(&ctx, key, keylen);
	md4_hmac_update(&ctx, input, ilen);
	md4_hmac_finish(&ctx, output);

	memset(&ctx, 0, sizeof(md4_context));
}

#if defined(TROPICSSL_SELF_TEST)

/*
 * RFC 1320 test vectors
 */
static const char md4_test_str[7][81] = {
	{""},
	{"a"},
	{"abc"},
	{"message digest"},
	{"abcdefghijklmnopqrstuvwxyz"},
	{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"},
	{
	 "12345678901234567890123456789012345678901234567890123456789012"
	 "345678901234567890"}
};

static const unsigned char md4_test_sum[7][16] = {
	{
	 0x31, 0xD6, 0xCF, 0xE0, 0xD1, 0x6A, 0xE9, 0x31,
	 0xB7, 0x3C, 0x59, 0xD7, 0xE0, 0xC0, 0x89, 0xC0},
	{
	 0xBD, 0xE5, 0x2C, 0xB3, 0x1D, 0xE3, 0x3E, 0x46,
	 0x24, 0x5E, 0x05, 0xFB, 0xDB, 0xD6, 0xFB, 0x24},
	{
	 0xA4, 0x48, 0x01, 0x7A, 0xAF, 0x21, 0xD8, 0x52,
	 0x5F, 0xC1, 0x0A, 0xE8, 0x7A, 0xA6, 0x72, 0x9D},
	{
	 0xD9, 0x13, 0x0A, 0x81, 0x64, 0x54, 0x9F, 0xE8,
	 0x18, 0x87, 0x48, 0x06, 0xE1, 0xC7, 0x01, 0x4B},
	{
	 0xD7, 0x9E, 0x1C, 0x30, 0x8A, 0xA5, 0xBB, 0xCD,
	 0xEE, 0xA8, 0xED, 0x63, 0xDF, 0x41, 0x2D, 0xA9},
	{
	 0x04, 0x3F, 0x85, 0x82, 0xF2, 0x41, 0xDB, 0x35,
	 0x1C, 0xE6, 0x27, 0xE1, 0x53, 0xE7, 0xF0, 0xE4},
	{
	 0xE3, 0x3B, 0x4D, 0xDC, 0x9C, 0x38, 0xF2, 0x19,
	 0x9C, 0x3E, 0x7B, 0x16, 0x4F, 0xCC, 0x05, 0x36}
};

/*
 * Checkup routine
 */
int md4_self_test(int verbose)
{
	int i;
	unsigned char md4sum[16];

	for (i = 0; i < 7; i++) {
		if (verbose != 0)
			printf("  MD4 test #%d: ", i + 1);

		md4((unsigned char *)md4_test_str[i],
		    strlen(md4_test_str[i]), md4sum);

		if (memcmp(md4sum, md4_test_sum[i], 16) != 0) {
			if (verbose != 0)
				printf("failed\n");

			return (1);
		}

		if (verbose != 0)
			printf("passed\n");
	}

	if (verbose != 0)
		printf("\n");

	return (0);
}

#endif

#endif
