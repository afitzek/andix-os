#include <trustlets/rsa_trustlet.h>
#include <tee_internal_api.h>
#include <tropicssl/rsa.h>
#include <tropicssl/pbkdf2.h>
#include <tropicssl/sha2.h>
//#include <rsa_internal.h>
#include <fcntl.h>
#include <swi.h>
#include <andix.h>
#include <client_constants.h>
#include <andix/entropy.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define MSG_ERROR "RSA Trustlet Error: "
#define MSG_INFO "RSA Trustlet Info: "
#define MSG_TROPIC_ERR "RSA Trustlet TropicSSL error "

enum keyselect {PRIVATE, PUBLIC};

// In contrast to polarSSL, tropicSSL expects this kind of RNG function.
static int f_rng_tropic_wrapper(void *in){
	int out;
	size_t olen;
	int ret = platform_pseudo_entropy(in, (unsigned char *)&out, sizeof(out), &olen);
	return out;
}

static TEE_Result write_mpi_fd(mpi *m, int fd) {
	int len = mpi_size(m) * 2 + 3;
	char linebuf[len];
	int ret = mpi_write_string(m, 16, linebuf, &len);
	if (ret) {
		printf(MSG_TROPIC_ERR "mpi_write_string -%x len %d\n", -ret, len);
		return TEE_ERROR_BAD_FORMAT;
	}
	if (*(linebuf + len - 1) != '\0') {	// check libraries behaviour
		printf(MSG_ERROR "String not 0 terminated\n");
		return TEE_ERROR_BAD_FORMAT;
	}
	ssize_t written = 0;
	while (written < len) {
		ret = write(fd, linebuf + written, len);
		if (ret < 0) {
			printf(MSG_ERROR "write() failed %d: %s\n", ret, strerror(errno));
			return TEE_ERROR_GENERIC;
		}
		written += ret;
	}
	return TEE_SUCCESS;
}
static TEE_Result writeKey(rsa_context *rsa, const char *name) {
	int ret;
	int fd = open(name, O_CREAT | O_WRONLY);
	if (fd < 0) {
		printf(MSG_ERROR "failed to open output file! %s\n", strerror(errno));
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	if ((ret = write_mpi_fd(&(rsa->N), fd )) ||
			(ret = write_mpi_fd(&(rsa->E), fd )) ||
			(ret = write_mpi_fd(&(rsa->D), fd )) ||
			(ret = write_mpi_fd(&(rsa->P), fd )) ||
			(ret = write_mpi_fd(&(rsa->Q), fd )) ||
			(ret = write_mpi_fd(&(rsa->DP), fd )) ||
			(ret = write_mpi_fd(&(rsa->DQ), fd)) ||
			(ret = write_mpi_fd(&(rsa->QP), fd ))) {
		printf(MSG_TROPIC_ERR "mpi_write_fd -%x\n", -ret);
		printf(MSG_ERROR "failed to write to output file!\n");
		close(fd);
		return TEE_ERROR_GENERIC;
	}
	close(fd);
	return TEE_SUCCESS;
}

static TEE_Result read_mpi_buf(mpi *m, char **p, char *end) {
	char *eos = *p + strlen(*p) + 1;
	if (eos >= end) {
		printf(MSG_ERROR "Buffer underrun!\n");
		return TEE_ERROR_BAD_FORMAT;
	}
	int ret = mpi_read_string(m, 16, *p);
	if (ret) {
		printf(MSG_TROPIC_ERR "mpi_read_string -%x\n", -ret);
		return TEE_ERROR_BAD_FORMAT;
	}
	*p = eos;
	return TEE_SUCCESS;
}

static TEE_Result readKey(rsa_context *rsa, const char *name) {
	int ret;
	struct stat stat;
	int fd = open(name, O_RDONLY);
	if (fd < 0) {
		printf(MSG_ERROR "failed to open input file! %s\n", strerror(errno));
		return TEE_ERROR_ITEM_NOT_FOUND;
	}
	ret = fstat(fd, &stat);
	if (ret) {
		printf(MSG_ERROR "failed to stat input file %d! %s\n", ret, strerror(errno));
		ret = TEE_ERROR_ITEM_NOT_FOUND;
		goto close;
	}
	printf(MSG_INFO "file size %s %ld\n", name, stat.st_size);
	if (stat.st_size == 0) {
		printf(MSG_ERROR "Empty files suck!\n");
		ret = TEE_ERROR_NO_DATA;
		goto close;
	}

	char *filebuf = malloc(stat.st_size + 1);
	char *filebufend = filebuf + stat.st_size + 1;
	ssize_t rlen = read(fd, filebuf, stat.st_size);
	if (rlen != stat.st_size) {
		printf(MSG_ERROR "failed to read input file! %d\n", rlen);
		ret = TEE_ERROR_NO_DATA;
		goto free;
	}

	char *p = filebuf;		// is advanced mpi-by-mpi
	if ((ret = read_mpi_buf(&(rsa->N), &p, filebufend)) ||
			(ret = read_mpi_buf(&(rsa->E), &p, filebufend)) ||
			(ret = read_mpi_buf(&(rsa->D), &p, filebufend)) ||
			(ret = read_mpi_buf(&(rsa->P), &p, filebufend)) ||
			(ret = read_mpi_buf(&(rsa->Q), &p, filebufend)) ||
			(ret = read_mpi_buf(&(rsa->DP), &p, filebufend)) ||
			(ret = read_mpi_buf(&(rsa->DQ), &p, filebufend)) ||
			(ret = read_mpi_buf(&(rsa->QP), &p, filebufend))) {

		printf(MSG_ERROR "failed to read from input file!\n");
		ret = TEE_ERROR_GENERIC;
		goto free;
	}
	rsa->len = mpi_size(&(rsa->N));
	ret = TEE_SUCCESS;

	free:
	free(filebuf);
	close:
	close(fd);
	return ret;
}

static TEE_Result createKeyPair(rsa_context *rsa, const char *name, unsigned int nbits, int pub_exponent) {
	int ret;
	ret = rsa_gen_key(rsa, nbits, pub_exponent);
	if (ret) {
		printf(MSG_TROPIC_ERR "rsa_gen_key -%x\n", -ret);
		return TEE_ERROR_GENERIC;
	}
	ret = rsa_check_privkey(rsa);
	if (ret) {
		printf(MSG_TROPIC_ERR "rsa_check_privkey -%x\n", -ret);
		return TEE_ERROR_GENERIC;
	}
	return writeKey(rsa, name);
}

static TEE_Result TA_createKeyPair(void* sessionContext, __uint32_t_ paramTypes, TEE_Param params[4]) {
	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INPUT,
			TEEC_VALUE_INPUT,
			TEEC_VALUE_INPUT,
			TEEC_NONE)) {
		printf(MSG_ERROR "Bad Parameters\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	rsa_context *rsa =  sessionContext;
	const char *name = params[0].memref.buffer;
	unsigned int nbits = params[1].value.a;
	int pub_expo = params[2].value.a;

	printf(MSG_INFO "Creating new key in file '%s'\n", name);
	return createKeyPair(rsa, name, nbits, pub_expo);
}

static TEE_Result TA_readKeyPair(void* sessionContext, __uint32_t_ paramTypes, TEE_Param params[4]) {
	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INPUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE)) {
		printf(MSG_ERROR "Bad Parameters\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	rsa_context *rsa =  sessionContext;
	const char *name = params[0].memref.buffer;

	int ret = readKey(rsa, name);
	if (ret)
		return ret;
	ret = rsa_check_privkey(rsa);
	if (ret) {
		printf(MSG_TROPIC_ERR "rsa_check_privkey -%x\n", -ret);
		return TEE_ERROR_BAD_FORMAT;
	}
	params[1].value.a = mpi_size(&(rsa->N));
	return TEE_SUCCESS;
}

static TEE_Result TA_getPubKey(void* sessionContext, __uint32_t_ paramTypes, TEE_Param params[4]) {
	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_MEMREF_TEMP_OUTPUT,
			TEEC_MEMREF_TEMP_OUTPUT,
			TEEC_NONE,
			TEEC_NONE)) {
		printf(MSG_ERROR "Bad Parameters\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	rsa_context *rsa =  sessionContext;
	if (rsa->len == 0) {	// no key loaded
		printf(MSG_ERROR "No key loaded in context!\n");
		return TEE_ERROR_BAD_STATE;
	}
	unsigned char *Ebuf = params[0].memref.buffer,
			*Nbuf = params[1].memref.buffer;
	size_t Elen = params[0].memref.size,
			Nlen = params[1].memref.size;

	if (mpi_write_binary(&(rsa->E), Ebuf, Elen) ||
			mpi_write_binary(&(rsa->N), Nbuf, Nlen)) {
		printf(MSG_ERROR "Destination buffer too small!\n");
		return TEE_ERROR_SHORT_BUFFER;
	}
	return TEE_SUCCESS;
}

static TEE_Result TA_applyRSAKey(enum keyselect key, void* sessionContext, __uint32_t_ paramTypes, TEE_Param params[4]) {
	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INPUT,
			TEEC_MEMREF_TEMP_OUTPUT,
			TEEC_NONE,
			TEEC_NONE)) {
		printf(MSG_ERROR "Bad Parameters\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	rsa_context *rsa =  sessionContext;
	if (rsa->len == 0) {	// no key loaded
		printf(MSG_ERROR "No key loaded in context!\n");
		return TEE_ERROR_BAD_STATE;
	}
	unsigned char *ibuf = params[0].memref.buffer,
				*obuf = params[1].memref.buffer;
		size_t ilen = params[0].memref.size,
				olen = params[1].memref.size;
	if (ilen < rsa->len || olen < rsa->len) {
		printf(MSG_ERROR "Destination buffer too small!\n");
		return TEE_ERROR_SHORT_BUFFER;
	}

	int ret;
	switch (key) {
	case PRIVATE:
		ret = rsa_private(rsa, ibuf, obuf);
		break;
	case PUBLIC:
		ret = rsa_public(rsa, ibuf, obuf);
		break;
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
	if (ret) {
		printf(MSG_TROPIC_ERR "rsa_(public|private) -%x\n", -ret);
		return TEE_ERROR_GENERIC;
	}
	return TEE_SUCCESS;
}

#ifdef INSECURE
#warning "The function TA_catKeyFile is enabled and reveals the private key! For testing purposes only!"
static TEE_Result TA_catKeyFile(void* sessionContext, __uint32_t_ paramTypes, TEE_Param params[4]) {
	char buf[64];
	ssize_t n;
	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INPUT,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE)) {
		printf(MSG_ERROR "Bad Parameters\n");
		/* Bad parameter types */
		return TEE_ERROR_BAD_PARAMETERS;
	}

	const char *name = params[0].memref.buffer;
	int fd = open(name, O_RDONLY);
	if (fd < 0) {
		printf(MSG_ERROR "failed to open input file!\n");
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	while ((n = read(fd, buf, sizeof(buf))) > 0) {
		write(1, buf, n);
	}
	close(fd);
	printf(MSG_ERROR "read returned %d: %s\n", n, strerror(errno));
	if (n == 0)
		return TEE_SUCCESS;
	if (n < 0) {
	}
	return TEE_ERROR_GENERIC;
}
#endif

void TA_CreateEntryPoint() {
	printf(MSG_INFO "Started\n");
}

void TA_DestroyEntryPoint() {
	printf(MSG_INFO "Destroyed\n");
}

TEE_Result TA_OpenSessionEntryPoint(__uint32_t_ paramTypes, TEE_Param params[4],
		void** sessionContext) {
//	rsa_context *rsa = TEE_Malloc(sizeof(rsa_context), 0);	///TODO seems like incompletely implemented.
	rsa_context *rsa = malloc(sizeof(rsa_context));
	if (rsa == NULL) {
		printf(MSG_ERROR "malloc failed!\n");
		return TEE_ERROR_OUT_OF_MEMORY;
	}
	rsa_init(rsa, RSA_PKCS_V15, 0, f_rng_tropic_wrapper, NULL);
	*sessionContext = rsa;
	printf(MSG_INFO "SESSION OPENED!\n");
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void* sessionContext) {
	rsa_free(sessionContext);
//	TEE_Free(sessionContext);
	free(sessionContext);
	printf(MSG_INFO "SESSION CLOSED!\n");
}

static const char *cmd2string(__uint32_t_ commandID) {
	switch (commandID) {
	case TZ_RSA_NEW_KEY:
		return TZ_RSA_NEW_KEY_STRING;
	case TZ_RSA_LOAD_KEY:
		return TZ_RSA_LOAD_KEY_STRING;
	case TZ_RSA_GET_PUBLIC_KEY:
		return TZ_RSA_GET_PUBLIC_KEY_STRING;
	case TZ_RSA_PRIVATE:
		return TZ_RSA_PRIVATE_STRING;
	case TZ_RSA_PUBLIC:
		return TZ_RSA_PUBLIC_STRING;
#ifdef INSECURE
	case TZ_RSA_CAT_KEY:
		return TZ_RSA_CAT_KEY_STRING;
#endif
	default:
		return "no string known for this command!";
	}
}
TEE_Result TA_InvokeCommandEntryPoint(void* sessionContext,
		__uint32_t_ commandID, __uint32_t_ paramTypes, TEE_Param params[4]) {

	printf(MSG_INFO "Parameter Types 0x%x\n", paramTypes);
	printf(MSG_INFO "Command invoked 0x%x %s\n", commandID, cmd2string(commandID));
	switch (commandID) {
	case TZ_RSA_NEW_KEY:
		return TA_createKeyPair(sessionContext, paramTypes, params);
	case TZ_RSA_LOAD_KEY:
		return TA_readKeyPair(sessionContext, paramTypes, params);
	case TZ_RSA_GET_PUBLIC_KEY:
		return TA_getPubKey(sessionContext, paramTypes, params);
	case TZ_RSA_PRIVATE:
		return TA_applyRSAKey(PRIVATE, sessionContext, paramTypes, params);
	case TZ_RSA_PUBLIC:
		return TA_applyRSAKey(PUBLIC, sessionContext, paramTypes, params);
#ifdef INSECURE
	case TZ_RSA_CAT_KEY:
		return TA_catKeyFile(sessionContext, paramTypes, params);
#endif
	default:
		return TEE_ERROR_NOT_SUPPORTED;
	}
}
