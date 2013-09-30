#include <stdio.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/objects.h>
#include <openssl/engine.h>
#include <openssl/dso.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/err.h>

#include <tee_client_api.h>
#include <trustlets/rsa_trustlet.h>
#include <tee_utils.h>

#define printerr(...)	fprintf(stderr, __VA_ARGS__)
#define printinfo(...)	printf(__VA_ARGS__)

#define eng_id "tz_rsa"		// in C this can't be static const char *..
#define eng_name "trustzone RSA engine"

static int rsa_ex_index;	// static index into RSA ex_data retrieved by the ex data API. Where we store our stuff.
struct tz_rsa_ex_t {
	TEEC_Context ctx;
	TEEC_Session session;
};

static void report_on_TEEerror(uint32_t origin, TEEC_Result result) {
	if (result) {
		printerr("TEE Invoke failed!\n");
		printerr("  ORIGIN: %s (0x%08X)\n", TEEC_StringifyOrigin(origin), origin);
		printerr("  ERROR : %s (0x%08X)\n", TEEC_StringifyError(result), result);
	}
}

int unknown_padding_add(void) {
	fprintf(stderr, "Padding not supported in tz_rsa_engine!\n");
	return -1;
}
int unknown_padding_check(void) {
	return unknown_padding_add();
}

static int tz_apply_private_key(int len,const unsigned char *inbuf, unsigned char *outbuf, RSA *rsa) {
	TEEC_Operation op;
	uint32_t origin = 0;
	TEEC_Result ret;
	struct tz_rsa_ex_t *tz_ref = RSA_get_ex_data(rsa, rsa_ex_index);
	int rsalen = RSA_size(rsa);

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,	//input
			TEEC_MEMREF_TEMP_OUTPUT,	//output
			TEEC_NONE,
			TEEC_NONE);
	op.params[0].tmpref.buffer = (unsigned char *)inbuf;
	op.params[0].tmpref.size = len;
	op.params[1].tmpref.buffer = outbuf;
	op.params[1].tmpref.size = rsalen;

	ret = TEEC_InvokeCommand(&(tz_ref->session), TZ_RSA_PRIVATE, &op, &origin);
	report_on_TEEerror(origin, ret);
	if (ret)	// openssl return value convention
		return -1;
	else
		return rsalen;
}

int tz_rsa_priv_enc(int flen,const unsigned char *from, unsigned char *to,
		RSA *rsa, int padding) {
	int rsalen = RSA_size(rsa);
	int ret;
	unsigned char *padded = OPENSSL_malloc(rsalen);
	if (padded == NULL)
		return -1;

	// like in openssl's rsa_eay.c
	switch (padding)
	{
	case RSA_PKCS1_PADDING:
		ret = RSA_padding_add_PKCS1_type_1(padded, rsalen, from, flen);
		break;
	case RSA_SSLV23_PADDING:
		ret = RSA_padding_add_SSLv23(padded, rsalen, from, flen);
		break;
	case RSA_NO_PADDING:
		ret = RSA_padding_add_none(padded, rsalen, from, flen);
		break;
	default:
		ret = unknown_padding_add();
	}
	if (ret > 0)
		ret = tz_apply_private_key(rsalen, padded, to, rsa);

	OPENSSL_free(padded);
	return ret;
}

int tz_rsa_priv_dec(int flen,const unsigned char *from, unsigned char *to,
		RSA *rsa,int padding) {
	int rsalen = RSA_size(rsa);
	int decrlen;
	unsigned char *decrypted = OPENSSL_malloc(rsalen);
	if (decrypted == NULL)
		return -1;

	decrlen = tz_apply_private_key(flen, from, decrypted, rsa);

	// Padding checkers (at least PKCS1) require the size to not include
	// top zero bytes. A valid PCKS1 padding should have this and the check
	// fails if decrlen is not rsalen-1, so we have to check it here.
	unsigned char *padin;
	if (*decrypted == 0) {
		decrlen--;
		padin = decrypted + 1;
	} else
		padin = decrypted;

	int olen;
	switch (padding) {
	case RSA_PKCS1_PADDING:
		olen = RSA_padding_check_PKCS1_type_2(to, rsalen, padin, decrlen, rsalen);
		break;
	case RSA_SSLV23_PADDING:
		olen = RSA_padding_check_SSLv23(to, rsalen, padin, decrlen, rsalen);
		break;
	case RSA_NO_PADDING:
		olen = RSA_padding_check_none(to, rsalen, padin, decrlen, rsalen);
		break;
	default:
		olen = unknown_padding_check();
	}

	OPENSSL_free(decrypted);
	return olen;
}
static struct tz_rsa_ex_t *tz_openSession(ENGINE *E) {
	struct tz_rsa_ex_t *tz_ref = OPENSSL_malloc(sizeof(struct tz_rsa_ex_t));
	TEEC_UUID uuid;
	TEEC_Result TEEret;
	uint32_t TEEorigin = 0;

	// open session
	TEE_uuid_parse(RSA_TRUSTLET_UUID, &uuid);
	TEEret = TEEC_InitializeContext(ANDIX_TEE_NAME, &(tz_ref->ctx));

	if (TEEret != TEEC_SUCCESS) {
		printerr("Failed to create context!\n");
		report_on_TEEerror(TEEorigin, TEEret);
		OPENSSL_free(tz_ref);
		return NULL;
	}

	TEEret = TEEC_OpenSession(&(tz_ref->ctx), &(tz_ref->session), &uuid, 0, NULL, NULL, &TEEorigin);

	if (TEEret != TEEC_SUCCESS) {
		printerr("Failed to create session!\n");
		report_on_TEEerror(TEEorigin, TEEret);
		TEEC_FinalizeContext(&(tz_ref->ctx));
		OPENSSL_free(tz_ref);
		return NULL;
	}
	return tz_ref;
}

static void tz_closeSession(struct tz_rsa_ex_t *tz_ref) {
	TEEC_CloseSession(&(tz_ref->session));
	TEEC_FinalizeContext(&(tz_ref->ctx));
	OPENSSL_free(tz_ref);
}

/*
 * Load a key in the trustlet and retrieve the public key and fill it into those
 * fancy cryptic EVP and RSA APIs that openssl gives us.
 * We also need to store a reference to the key in the trustlet to connect them somehow.
 * And we need to place the right references to the engine and it's methods.
 */
static EVP_PKEY *tz_loadKey(ENGINE *E, const char *name, UI_METHOD *ui_method, void *callback_data) {
	TEEC_UUID uuid;
	TEEC_Result TEEret;
	uint32_t TEEorigin = 0;

	// open session
	struct tz_rsa_ex_t *tz_ref = tz_openSession(E);
	if (tz_ref == NULL) {
		return NULL;
	}

	// load key in TZ
	TEEC_Operation TEEop;
	int rsalen;
	printinfo("Loading RSA key '%s'\n", name);
	TEEop.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INPUT,	//keyfile
			TEEC_VALUE_OUTPUT,	// byte length of modulus
			TEEC_NONE,
			TEEC_NONE);
	TEEop.params[0].tmpref.buffer = (char *) name;
	TEEop.params[0].tmpref.size = strlen(name) + 1;

	TEEret = TEEC_InvokeCommand(&(tz_ref->session), TZ_RSA_LOAD_KEY, &TEEop, &TEEorigin);
	report_on_TEEerror(TEEorigin, TEEret);
	rsalen = TEEop.params[1].value.a;
	printinfo("Key '%s' loaded. Modulus len is %d bytes\n", name, rsalen);

	// retrieve public key from TZ
	unsigned char bufN[rsalen], bufE[rsalen];
	memset(bufN, 0, rsalen);
	memset(bufE, 0, rsalen);

	TEEop.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,	//E
			TEEC_MEMREF_TEMP_OUTPUT,	//N
			TEEC_NONE,
			TEEC_NONE);
	TEEop.params[0].tmpref.buffer = bufE;
	TEEop.params[0].tmpref.size = rsalen;
	TEEop.params[1].tmpref.buffer = bufN;
	TEEop.params[1].tmpref.size = rsalen;

	TEEret = TEEC_InvokeCommand(&(tz_ref->session), TZ_RSA_GET_PUBLIC_KEY, &TEEop, &TEEorigin);
	report_on_TEEerror(TEEorigin, TEEret);

	// Init EVP and RSA stuff
	EVP_PKEY *pkey = EVP_PKEY_new();
	RSA *rsa = RSA_new_method(E);	// in contrast to RSA_new() this should connect with this engine.
	rsa->n = BN_bin2bn(bufN, rsalen, NULL);
	rsa->e = BN_bin2bn(bufE, rsalen, NULL);

	// save the TZ session with the RSA object.
	// Make sure the session is closed when the RSA object is destroyed. (tz_rsa_ex_free should do that).
	RSA_set_ex_data(rsa, rsa_ex_index, tz_ref);

	// put the RSA object into the EVP_PKEY object and give up our reference.
	EVP_PKEY_set1_RSA(pkey, rsa);
	RSA_free(rsa);
	return pkey;
}

int tz_rsa_keygen(RSA *rsa, int bits, BIGNUM *e, BN_GENCB *cb) {
	fprintf(stderr, "Error: RSA key generation is not implemented via the RSA API!\n");
	return 0;
}

int tz_rsa_new_key(ENGINE *e, long rsabitlen, const char *keyfilename) {
	TEEC_Operation op;
	uint32_t origin = 0;
	TEEC_Result ret;

	struct tz_rsa_ex_t *tz_ref = tz_openSession(e);
	if (tz_ref == NULL) {
		return 0;
	}
	printf("Creating new RSA key '%s'\n", keyfilename);
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,	//keyfile
			TEEC_VALUE_INPUT,	// bit size
			TEEC_VALUE_INPUT,	// public exponent
			TEEC_NONE);
	op.params[0].tmpref.buffer = (char *)keyfilename;
	op.params[0].tmpref.size = strlen(keyfilename) + 1;

	op.params[1].value.a = rsabitlen;
	op.params[2].value.a = 65537;

	ret = TEEC_InvokeCommand(&(tz_ref->session), TZ_RSA_NEW_KEY, &op, &origin);
	report_on_TEEerror(origin, ret);
	tz_closeSession(tz_ref);
	if (ret == TEEC_SUCCESS)	// boolean return value required
		return 1;
	else
		return 0;
}

RSA_METHOD tz_rsa_methods= {
		eng_id,
		NULL, /* rsa_pub_enc (wrap) */
		NULL, /* rsa_pub_dec (verification) */
		tz_rsa_priv_enc, /* rsa_priv_enc (signing) */
		tz_rsa_priv_dec, /* rsa_priv_dec (unwrap) */
		NULL, /* rsa_mod_exp */
		NULL, /* bn_mod_exp */
		NULL, /* init */
		NULL, /* finish */
		RSA_FLAG_EXT_PKEY | RSA_FLAG_NO_BLINDING, /* flags */
		NULL, /* app_data */
		NULL, /* rsa_sign */
		NULL, /* rsa_verify */
		tz_rsa_keygen, /* rsa_keygen */
};

/* Engines can have additional configuration commands. We don't use them. */
static const ENGINE_CMD_DEFN eng_ctrl_cmd_defns[] = {
		{ENGINE_CMD_BASE, "newkey",
				"Create a new key with the given name and length (bits) in the TrustZone.",
				ENGINE_CMD_FLAG_NUMERIC | ENGINE_CMD_FLAG_STRING},
		{0, NULL, NULL, 0}
};

int tz_rsa_ctrl(ENGINE *e, int cmd, long rsabitlen, void *name, void (*f)(void)) {
	switch (cmd) {
	case ENGINE_CMD_BASE:
		return tz_rsa_new_key(e, rsabitlen, name);
	default:
		return ENGINE_R_CTRL_COMMAND_NOT_IMPLEMENTED;
	}
}

/* called when a RSA object is created/duplicated/freed to manage our ex_data. */
/* returning 1 means success, 0 means failure! */
int tz_rsa_ex_new(void *parent, void *ptr, CRYPTO_EX_DATA *ad, int idx, long argl, void *argp) {
	return CRYPTO_set_ex_data(ad, idx, NULL);	// set to NULL for now
}
int tz_rsa_ex_dup(CRYPTO_EX_DATA *to, CRYPTO_EX_DATA *from, void *from_d,
		int idx, long argl, void *argp) {
//	struct tz_rsa_ex_t *new_ex = OPENSSL_malloc(sizeof(struct tz_rsa_ex_t));
	// openssl doc says this is never called anyways.
	return 0;
}

void tz_rsa_ex_free(void *parent, void *ptr, CRYPTO_EX_DATA *ad, int idx, long argl, void *argp) {
	struct tz_rsa_ex_t *tz_ref = CRYPTO_get_ex_data(ad, idx);
	tz_closeSession(tz_ref);
}

static void setup_rsa() {
	const RSA_METHOD* rsa_meth = RSA_get_default_method();

	// use the default methods where we don't override them.
	tz_rsa_methods.rsa_pub_enc = rsa_meth->rsa_pub_enc;
	tz_rsa_methods.rsa_pub_dec = rsa_meth->rsa_pub_dec;
	tz_rsa_methods.rsa_mod_exp = rsa_meth->rsa_mod_exp;
	tz_rsa_methods.bn_mod_exp = rsa_meth->bn_mod_exp;

	// get an ex_data index. This provides some means of storing application specific data.
	rsa_ex_index = RSA_get_ex_new_index(0, NULL, tz_rsa_ex_new, tz_rsa_ex_dup, tz_rsa_ex_free);
}

static int bind_fn(ENGINE * e, const char *id) {
	setup_rsa();

	if (!ENGINE_set_id(e, eng_id) ||
			!ENGINE_set_name(e, eng_name) ||
			!ENGINE_set_RSA(e, &tz_rsa_methods) ||
			!ENGINE_set_load_privkey_function(e, tz_loadKey)||
			!ENGINE_set_load_pubkey_function(e, tz_loadKey) ||
			!ENGINE_set_flags(e, 0) ||
			!ENGINE_set_cmd_defns(e, eng_ctrl_cmd_defns) ||
			!ENGINE_set_ctrl_function(e, tz_rsa_ctrl)) {

	}
}

IMPLEMENT_DYNAMIC_CHECK_FN();
IMPLEMENT_DYNAMIC_BIND_FN(bind_fn);
