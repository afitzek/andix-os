#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <openssl/engine.h>
#include <openssl/pem.h>

#define RSABITLEN 512
#define RSABYTELEN (RSABITLEN/8)

static const char *engine_path = "/deploy/norm/lib/libtz_rsa_engine.so";

//int newKey(char *);
int privateKey(char *keyname, ENGINE *e);
int publicKey(char *keyname, ENGINE *e);
int getPubKey(char *keyname, ENGINE *e);
int newKey(char *keyname, ENGINE *e);
int privateKeyEncPKCS1(char *keyname, ENGINE *e);
int privateKeyDecPKCS1(char *keyname, ENGINE *e);
int publicKeyEncPKCS1(char *keyname, ENGINE *e);
int publicKeyDecPKCS1(char *keyname, ENGINE *e);
int privateKeyDecSSLV23(char *keyname, ENGINE *e);
int privateKeyEncSSLV23(char *keyname, ENGINE *e);
int publicKeyDecSSLV23(char *keyname, ENGINE *e);
int publicKeyEncSSLV23(char *keyname, ENGINE *e);

static const char const *cmds[] = {
		"priv",
		"pub",
		"privdecpk1",
		"privencpk1",
		"pubdecpk1",
		"pubencpk1",
		"privdecssl",
		"privencssl",
		"pubdecssl",
		"pubencssl",
		"newkey",
		"getpubkey",
};

static const int ncmds = sizeof(cmds) / sizeof(const char *);
typedef int (*cmdfunc_t)(char *, ENGINE *);
static const cmdfunc_t cmdfuncs[] = {
		privateKey,
		publicKey,
		privateKeyDecPKCS1,
		privateKeyEncPKCS1,
		publicKeyDecPKCS1,
		publicKeyEncPKCS1,
		privateKeyDecSSLV23,
		privateKeyEncSSLV23,
		publicKeyDecSSLV23,
		publicKeyEncSSLV23,
		newKey,
		getPubKey,
};

void usage(char* argv[]) {
	printf("Test tool for the openssl RSA trustlet engine.\nCommands [%i]:\n", ncmds);
	int i;
	for (i=0; i < ncmds; i++) {
		printf("%s %s <keyname>\n", argv[0], cmds[i]);
	}
}

void printHexMem(unsigned char* buffer, size_t size) {
	size_t idx = 0;
	for (idx = 0; idx < size; idx++) {
		if (idx > 0 && idx % 16 == 0) {
			printf("\n");
		}
		printf("%02X ", buffer[idx]);
	}
	printf("\n");
}

static RSA *loadKey(char *keyname, ENGINE *e) {
	EVP_PKEY *key = ENGINE_load_private_key(e, keyname, NULL, NULL);
	RSA *rsa = NULL;
	if (key != NULL) {
		rsa = EVP_PKEY_get1_RSA(key);
		EVP_PKEY_free(key);
	}
	if (key == NULL || rsa == NULL) {
		printf("Loading key '%s' failed!\n", keyname);
		return NULL;
	}
	printf("%s:\nN:", keyname);
	BN_print_fp(stdout, rsa->n);
	printf("\nE:");
	BN_print_fp(stdout, rsa->e);
	printf("\n");
	return rsa;
}


enum modeselektor {PRIVATE_ENC, PRIVATE_DEC, PUBLIC_ENC, PUBLIC_DEC};
int applyKey(char *keyname, ENGINE *e, enum modeselektor mode, int padding, int len_for_pad) {
	int ret;
	RSA *rsa = loadKey(keyname, e);
	if (rsa == NULL)
		return -1;

	int rsalen = RSA_size(rsa);
	unsigned char inbuf[rsalen], outbuf[rsalen];
	size_t inbytes;

	printf("Reading input..\n");
	memset(inbuf, 0, rsalen);
	memset(outbuf, 0, rsalen);
	{
		char *p = inbuf;
		int ret;
		for (inbytes=0; inbytes< rsalen - len_for_pad; ) {
			unsigned int in;
			ret = scanf("%02x ", &in);
			if (ret == 1) {
				inbytes++;
				*(p++) = in;
			}
			if (ret == EOF)
				break;
		}
	}
	printf("\nInput [%d]:\n", inbytes);
	printHexMem(inbuf, rsalen);

	switch (mode) {
	case PRIVATE_ENC:
		ret = RSA_private_encrypt(rsalen - len_for_pad, inbuf, outbuf, rsa, padding) < 0; 	// < 0 means error in openssl
		break;
	case PRIVATE_DEC:
		ret = RSA_private_decrypt(rsalen - len_for_pad, inbuf, outbuf, rsa, padding) < 0;
		break;
	case PUBLIC_ENC:
		ret = RSA_public_encrypt(rsalen - len_for_pad, inbuf, outbuf, rsa, padding) < 0;
		break;
	case PUBLIC_DEC:
		ret = RSA_public_decrypt(rsalen - len_for_pad, inbuf, outbuf, rsa, padding) < 0;
		break;
	}

	printf("\nOutput:\n");
	printHexMem(outbuf, rsalen);
	RSA_free(rsa);
	return ret;
}

int privateKey(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PRIVATE_ENC, RSA_NO_PADDING, 0);
}

int publicKey(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PUBLIC_ENC, RSA_NO_PADDING, 0);
}

int privateKeyDecPKCS1(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PRIVATE_DEC, RSA_PKCS1_PADDING, 0);
}

int privateKeyEncPKCS1(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PRIVATE_ENC, RSA_PKCS1_PADDING, RSA_PKCS1_PADDING_SIZE);
}

int publicKeyDecPKCS1(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PUBLIC_DEC, RSA_PKCS1_PADDING, 0);
}

int publicKeyEncPKCS1(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PUBLIC_ENC, RSA_PKCS1_PADDING, RSA_PKCS1_PADDING_SIZE);
}

int privateKeyDecSSLV23(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PRIVATE_DEC, RSA_SSLV23_PADDING, 0);
}

int privateKeyEncSSLV23(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PRIVATE_ENC, RSA_SSLV23_PADDING, 11);
}

int publicKeyDecSSLV23(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PUBLIC_DEC, RSA_SSLV23_PADDING, 0);
}

int publicKeyEncSSLV23(char *keyname, ENGINE *e) {
	return applyKey(keyname, e, PUBLIC_ENC, RSA_SSLV23_PADDING, 11);
}

int getPubKey(char *keyname, ENGINE *e) {
	FILE *file = fopen(keyname, "w");
	if (file == NULL)
		return -1;

	RSA *rsa = loadKey(keyname, e);
	if (rsa == NULL)
		return 1;
	PEM_write_RSAPublicKey(file, rsa);
	RSA_free(rsa);
	fclose(file);
	printf("Public key written to file '%s'\n", keyname);
	return 0;
}

int newKey(char *keyname, ENGINE *e) {
	int ret = ENGINE_ctrl_cmd(e, "newkey", RSABITLEN, keyname, NULL, 0);
	if (ret)
		printf("Successfully created new key '%s'.\n", keyname);
	else
		printf("Failed to create new key '%s'.\n", keyname);
	return !ret;
}

int main(int argc, char *argv[]) {
	int i, ret = 0;
	ENGINE *e = NULL;
	EVP_PKEY *key = NULL;

	if (argc != 3) {
		usage(argv);
		return -1;
	}
	char *keyname = argv[2];

	ERR_load_RSA_strings();
	ENGINE_load_dynamic();
	e= ENGINE_by_id("dynamic");
	if (e == NULL) {
		printf("Failed to get 'dynamic' engine!\n");
		ret = 1;
		goto exit;
	}

	ret = ENGINE_ctrl_cmd_string(e, "SO_PATH", engine_path, 0);
	if (!ret) {
		printf("Failed to SO_PATH '%s'\n", engine_path);
		ret = 2;
		goto exit;
	}
	ret = ENGINE_ctrl_cmd_string(e, "LOAD", NULL, 0);
	if (!ret) {
		printf("Failed to LOAD '%s'\n", engine_path);
		ret = 3;
		goto exit;
	}
	printf("Engine '%s' loaded!\n", engine_path);

	ret = ENGINE_init(e);
	if (!ret) {
		printf("Failed to ENGINE_init\n");
		ret = 4;
		goto exit;
	}

	for (i=0; i < ncmds; i++) {
		if (strcmp(argv[1], cmds[i]) == 0) {
			ret = cmdfuncs[i](keyname, e);
			break;
		}
	}
	if (i == ncmds) {
		printf("Command not found!\n");
		usage(argv);
		ret = -1;
	}
	if (ret) {
		printf("Command failed.\n");
	}

	exit:
	ERR_print_errors_fp(stderr);
	ENGINE_finish(e);
	printf("\nreturning %d\n", ret);
	return ret;
}
