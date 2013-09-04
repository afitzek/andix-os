
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tee_client_api.h>
#include <trustlets/rsa_trustlet.h>
#include <tee_utils.h>

#define RSABITLEN 512
#define RSABYTELEN (RSABITLEN/8)

static TEEC_Context ctx;
static TEEC_Session session;

int newKey(char *);
int privateKey(char *);
int publicKey(char *);
int getPubKey(char *);
int catKeyFile(char *);

static const char const *cmds[] = {
		"priv",
		"pub",
		"newkey",
		"getpubkey",
		"cat"
};

static const int ncmds = sizeof(cmds) / sizeof(const char *);
typedef int (*cmdfunc_t)(char *);
static const cmdfunc_t cmdfuncs[] = {
		privateKey,
		publicKey,
		newKey,
		getPubKey,
		catKeyFile
};

void usage(char* argv[]) {
	printf("Test tool for the RSA trustlet.\nCommands [%i]:\n", ncmds);
	int i;
	for (i=0; i < ncmds; i++) {
		printf("%s %s <keyname>\n", argv[0], cmds[i]);
	}
}

void report_on_error(uint32_t origin, TEEC_Result result) {
	if (result) {
		printf("Invoke failed!\n");
		printf("  ORIGIN: %s (0x%08X)\n", TEEC_StringifyOrigin(origin), origin);
		printf("  ERROR : %s (0x%08X)\n", TEEC_StringifyError(result), result);
	}
}

void printHexMem(unsigned char* buffer, size_t size) {
	size_t idx = 0;
	printf("0x%08X:  ", idx);
	for (idx = 0; idx < size; idx++) {
		if (idx > 0 && idx % 16 == 0) {
			printf("\n0x%08X:  ", idx);
		}
		printf("%02X ", buffer[idx]);
	}
	printf("\n");
}

int newKey(char *keyfilename) {
	TEEC_Operation op;
	uint32_t origin = 0;
	TEEC_Result ret;

	printf("Creating new RSA key '%s'\n", keyfilename);
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,	//keyfile
			TEEC_VALUE_INPUT,	// bit size
			TEEC_VALUE_INPUT,	// public exponent
			TEEC_NONE);
	op.params[0].tmpref.buffer = keyfilename;
	op.params[0].tmpref.size = strlen(keyfilename) + 1;

	op.params[1].value.a = RSABITLEN;
	op.params[2].value.a = 65537;

	ret = TEEC_InvokeCommand(&session, TZ_RSA_NEW_KEY, &op, &origin);
	report_on_error(origin, ret);
	return ret;
}

int loadKey(char *keyfilename, int *len) {
	TEEC_Operation op;
	uint32_t origin = 0;
	TEEC_Result ret;

	printf("Loading RSA key '%s'\n", keyfilename);
	op.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INPUT,	//keyfile
			TEEC_VALUE_OUTPUT,	// byte length of modulus
			TEEC_NONE,
			TEEC_NONE);
	op.params[0].tmpref.buffer = keyfilename;
	op.params[0].tmpref.size = strlen(keyfilename) + 1;

	ret = TEEC_InvokeCommand(&session, TZ_RSA_LOAD_KEY, &op, &origin);
	report_on_error(origin, ret);
	if (len)
		*len = op.params[1].value.a;
	return ret;
}

enum keyselect {PRIVATE, PUBLIC};
int applyKey(char *keyfilename, enum keyselect key) {
	TEEC_Operation op;
	uint32_t origin = 0;
	TEEC_Result ret;
	unsigned char inbuf[RSABYTELEN], outbuf[RSABYTELEN];
	size_t inbytes;

	ret = loadKey(keyfilename, NULL);
	if (ret)
		return ret;
	printf("Loading key ok. Waiting for input..\n");

	memset(inbuf, 0, RSABYTELEN);
//	inbytes = read(0, inbuf, RSABYTELEN);
	{
		char *p = inbuf;
		int ret;
		for (inbytes=0; inbytes<RSABYTELEN; ) {
			unsigned int in;
			ret = scanf("%02x ", &in);
			if (ret == 1) {
				inbytes++;
				*p = in;
				p++;
			}
			if (ret == EOF)
				break;
		}
	}
	printf("\nInput [%d]:\n", inbytes);
	printHexMem(inbuf, RSABYTELEN);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,	//input
			TEEC_MEMREF_TEMP_OUTPUT,	//output
			TEEC_NONE,
			TEEC_NONE);
	op.params[0].tmpref.buffer = inbuf;
	op.params[0].tmpref.size = RSABYTELEN;
	op.params[1].tmpref.buffer = outbuf;
	op.params[1].tmpref.size = RSABYTELEN;

	switch (key) {
	case PUBLIC:
		ret = TEEC_InvokeCommand(&session, TZ_RSA_PUBLIC, &op, &origin);
		break;
	case PRIVATE:
		ret = TEEC_InvokeCommand(&session, TZ_RSA_PRIVATE, &op, &origin);
		break;
	default:
		break;
	}
	report_on_error(origin, ret);

	printf("\nOutput:\n");
	printHexMem(outbuf, RSABYTELEN);
	return ret;
}

int privateKey(char *keyfilename) {
	return applyKey(keyfilename, PRIVATE);
}
int publicKey(char *keyfilename) {
	return applyKey(keyfilename, PUBLIC);
}
int getPubKey(char *keyfilename) {
	TEEC_Operation op;
	uint32_t origin = 0;
	TEEC_Result ret;
	int rsalen;

	ret = loadKey(keyfilename, &rsalen);
	if (ret)
		return ret;
	printf("Loading key ok, len is %d.\n", rsalen);
	if (!(rsalen > 0 && rsalen < 0xfff)) {
		printf("RSA length seems stupid!\n");
		return TEEC_ERROR_BAD_FORMAT;
	}

	unsigned char bufN[rsalen], bufE[rsalen];
	memset(bufN, 0, rsalen);
	memset(bufE, 0, rsalen);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,	//E
			TEEC_MEMREF_TEMP_OUTPUT,	//N
			TEEC_NONE,
			TEEC_NONE);
	op.params[0].tmpref.buffer = bufE;
	op.params[0].tmpref.size = rsalen;
	op.params[1].tmpref.buffer = bufN;
	op.params[1].tmpref.size = rsalen;

	ret = TEEC_InvokeCommand(&session, TZ_RSA_GET_PUBLIC_KEY, &op, &origin);
	report_on_error(origin, ret);
	if (!ret) {
		printf("E:\n");
		printHexMem(bufE, rsalen);
		printf("N:\n");
		printHexMem(bufN, rsalen);
	}
	return ret;
}

int catKeyFile(char *keyfilename) {
	TEEC_Operation op;
	uint32_t origin = 0;
	TEEC_Result ret;

	printf("Loading RSA key '%s'\n", keyfilename);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,	//keyfile
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE);
	op.params[0].tmpref.buffer = keyfilename;
	op.params[0].tmpref.size = strlen(keyfilename) + 1;

	ret = TEEC_InvokeCommand(&session, TZ_RSA_CAT_KEY, &op, &origin);
	report_on_error(origin, ret);
	return ret;
}

int main(int argc, char *argv[]) {
	TEEC_UUID uuid;
	TEEC_Result TEE_ret;
	uint32_t origin = 0;
	int i, ret = 0;

	if (argc != 3) {
		usage(argv);
		return -1;
	}
	uuid_parse(RSA_TRUSTLET_UUID, &uuid);

	TEE_ret = TEEC_InitializeContext(ANDIX_TEE_NAME, &ctx);

	if (TEE_ret != TEEC_SUCCESS) {
		printf("Failed to create context!\n");
		report_on_error(origin, TEE_ret);
		return 1;
	}

	TEE_ret = TEEC_OpenSession(&ctx, &session, &uuid, 0, NULL, NULL, &origin);

	if (TEE_ret != TEEC_SUCCESS) {
		printf("Failed to create session!\n");
		report_on_error(origin, TEE_ret);
		TEEC_FinalizeContext(&ctx);
		return 2;
	}

	for (i=0; i < ncmds; i++) {
		if (strcmp(argv[1], cmds[i]) == 0) {
			ret = cmdfuncs[i](argv[2]);
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

	TEEC_CloseSession(&session);
	TEEC_FinalizeContext(&ctx);

	printf("\nreturning %d\n", ret);
	return ret;
}
