/*
 * fs.c
 *
 *  Created on: Mar 16, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <fs/fs.h>
#include <common.h>
#include <fs/ree_fs.h>
#include <tropicssl/aes.h>
#include <tropicssl/sha2.h>
#include <tropicssl/pbkdf2.h>
#include <devices/random/random.h>

uint8_t* secret_value;
uint32_t secret_length;

uint32_t init_magic = 0;

int32_t __fs_check_init() {
	return (init_magic == CRYPTO_FS_INIT_MAGIC);
}

uint8_t __fs_decrypt_and_verify(cfs_blk_t *cfs, uint8_t* key, uint32_t klen,
		cfs_data_t* outputdata, uint32_t blknum) {
	aes_context aes_ctx;
	sha2_context sha_ctx;
	uint8_t digest[32];
	uint8_t derkey[32];
	uint8_t hash[32];
	uint8_t iv[16];

	// decrypt key ...
	memset(digest, 0, 32);
	memset(hash, 0, 32);
	memset(&aes_ctx, 0, sizeof(aes_ctx));
	memset(&sha_ctx, 0, sizeof(sha_ctx));

	// derive key
	sha2_starts(&sha_ctx, 0);
	sha2_update(&sha_ctx, cfs->iv, 16);
	sha2_update(&sha_ctx, (uint8_t*) &blknum, 4);
	sha2_update(&sha_ctx, key, klen);
	sha2_finish(&sha_ctx, derkey);
	memcpy(iv, cfs->iv, 16);

	// set decryption key
	aes_setkey_dec(&aes_ctx, derkey, 256);

	// reset sha context
	memset(&sha_ctx, 0, sizeof(sha_ctx));

	// calculate hmac over encrypted data!
	sha2_hmac_starts(&sha_ctx, derkey, 32, 0);
	sha2_hmac_update(&sha_ctx, (uint8_t*) &(cfs->data.blk), sizeof(cfs_data_t));
	sha2_hmac_update(&sha_ctx, (uint8_t*) cfs->hash, 32);
	sha2_hmac_finish(&sha_ctx, digest);

	// check hmac
	if (memcmp(digest, cfs->hmac, 32) != 0) {
		crypto_fs_error("Block verification failed!");
		return (-1);
	}

	// decrypt data
	aes_crypt_cbc(&aes_ctx, AES_DECRYPT, sizeof(cfs_data_t), iv,
			(uint8_t*) &(cfs->data.blk), (uint8_t*) outputdata);

	// reset sha context
	memset(&sha_ctx, 0, sizeof(sha_ctx));
	sha2_starts(&sha_ctx, 0);
	sha2_update(&sha_ctx, (uint8_t*) outputdata, sizeof(cfs_data_t));
	sha2_finish(&sha_ctx, hash);

	// check
	if (memcmp(hash, cfs->hash, 32) != 0) {
		crypto_fs_error("Block plaintext verification failed! (BAD DECRYPT)");
		return (-1);
	}

	// ok
	return (0);
}

uint8_t __fs_encrypt_and_sign(cfs_blk_t *cfs, uint8_t* key, uint32_t klen,
		cfs_data_t* inputdata, uint32_t blknum) {
	aes_context aes_ctx;
	sha2_context sha_ctx;
	uint8_t digest[32];
	uint8_t derkey[32];
	uint8_t iv[16];

	// reset variables
	memset(digest, 0, 32);
	memset(&aes_ctx, 0, sizeof(aes_ctx));
	memset(&sha_ctx, 0, sizeof(sha_ctx));

	// derive key
	sha2_starts(&sha_ctx, 0);
	sha2_update(&sha_ctx, cfs->iv, 16);
	sha2_update(&sha_ctx, (uint8_t*) &blknum, 4);
	sha2_update(&sha_ctx, key, klen);
	sha2_finish(&sha_ctx, derkey);

	// calculate hash over crypto data
	sha2((uint8_t*) inputdata, sizeof(cfs_data_t), cfs->hash, 0);

	// copy IV
	memcpy(iv, cfs->iv, 16);

	// set encryption key
	aes_setkey_enc(&aes_ctx, derkey, 256);

	// encrypt data
	aes_crypt_cbc(&aes_ctx, AES_ENCRYPT, sizeof(cfs_data_t), iv,
			(uint8_t*) inputdata, (uint8_t*) &(cfs->data.blk));
	// reset hash context
	memset(&sha_ctx, 0, sizeof(sha_ctx));

	// sign the data block with secret key
	sha2_hmac_starts(&sha_ctx, derkey, 32, 0);
	sha2_hmac_update(&sha_ctx, (uint8_t*) &(cfs->data.blk), sizeof(cfs_data_t));
	sha2_hmac_update(&sha_ctx, (uint8_t*) cfs->hash, 32);
	sha2_hmac_finish(&sha_ctx, digest);
	memcpy(cfs->hmac, digest, 32);

	return (0);
}

void fs_set_secret(uint8_t* secret, uint32_t len) {
	uint32_t udid = getUDID();
	secret_value = (uint8_t*) kmalloc(32);

	if (secret_value == NULL ) {
		crypto_fs_error("Failed to allocate memory!");
		kpanic();
	}

	pbkdf2_sha256_hmac(secret, len, (uint8_t*) &udid, 4, 8096, 32, secret_value);

	secret_length = 32;

	init_magic = CRYPTO_FS_INIT_MAGIC;
}

int32_t fs_remove(uint8_t *storename, uint32_t slen, uint8_t *filename,
		uint32_t flen) {
	uint8_t store_hash[TZ_FS_HASH_SIZE];
	uint8_t file_hash[TZ_FS_HASH_SIZE];

	sha2(storename, slen, store_hash, 0);
	sha2(filename, flen, file_hash, 0);

	return (ree_remove(store_hash, file_hash));
}

int32_t fs_open(uint8_t *storename, uint32_t slen, uint8_t *filename,
		uint32_t flen, int32_t flags, int32_t mode, cfs_t* handle) {

	uint8_t store_hash[TZ_FS_HASH_SIZE];
	uint8_t file_hash[TZ_FS_HASH_SIZE];
	cfs_blk_t blk;
	cfs_data_t data;
	fs_stat stat;
	int32_t fd = -1;

	if (!__fs_check_init()) {
		fs_error("FS not initialized");
		return (-1);
	}

	sha2(storename, slen, store_hash, 0);
	sha2(filename, flen, file_hash, 0);

	fd = ree_open(store_hash, file_hash, flags, mode);

	if (fd <= 0) {
		fs_error("Failed to open file!");
		return (-1);
	}

	handle->fd = fd;
	handle->filepos = 0;

	//check file size: if size = 0 create key and header

	if (ree_fstat(fd, &stat) != 0) {
		fs_error("Failed to stat file!");
		goto cleanup;
	}

	fs_info("File size %d", stat.st_size);

	if (stat.st_size == 0) {
		// create first block

		// set IV
		random_fill(blk.iv, 16);

		// set key for file
		random_fill(data.info.key_data, 32);
		memcpy(handle->key_data, data.info.key_data, 32);

		memcpy(data.info.storename, store_hash, TZ_FS_HASH_SIZE);
		memcpy(data.info.filename, file_hash, TZ_FS_HASH_SIZE);

		// encrypt and sign
		if (__fs_encrypt_and_sign(&blk, secret_value, secret_length, &data, 0)
				!= 0) {
			fs_error("Failed to encrypt and sign data");
			goto cleanup;
		}

		if (ree_write(handle->fd, (void*) &blk, sizeof(cfs_blk_t))
				!= sizeof(cfs_blk_t)) {
			fs_error("Failed to write data");
			goto cleanup;
		}
	} else {
		// read first block to get key
		if (ree_read(handle->fd, (void*) &blk, sizeof(cfs_blk_t))
				!= sizeof(cfs_blk_t)) {
			fs_error("Failed to read meta block");
			goto cleanup;
		}

		if (__fs_decrypt_and_verify(&blk, secret_value, secret_length, &data, 0)
				!= 0) {
			fs_error("Failed to decrypt and verify data");
			goto cleanup;
		}

		//set handle key
		memcpy(handle->key_data, data.info.key_data, 32);
	}

	return (0);

	cleanup: if (fd > 0) {
		handle->fd = fd;
		// close file
		fs_close(handle);
		handle->fd = -1;
	}

	handle->fd = -1;
	memset(handle->key_data, 0, 32);

	return (-1);
}

int32_t fs_close(cfs_t* handle) {
	return (ree_close(handle->fd));
}

int32_t fs_read(cfs_t* handle, void* buf, uint32_t count) {
	cfs_blk_t blk;
	cfs_data_t data;
	uint32_t curFilepos = handle->filepos;

	uint8_t* curBuffer = buf;

	if (!__fs_check_init()) {
		fs_error("FS not initialized");
		return (-1);
	}

	uint32_t left = count;
	// start reading at handle->filepos for count times
	uint32_t totalread = 0;
	uint32_t blknum = handle->filepos / CFS_BLK_SIZE;
	blknum = blknum + 1; // ignore first block
	uint32_t blkoffset = handle->filepos % CFS_BLK_SIZE;

	do {
		blknum = handle->filepos / CFS_BLK_SIZE;
		blknum = blknum + 1; // ignore first block
		blkoffset = handle->filepos % CFS_BLK_SIZE;
		fs_info("Reading blk #%d", blknum);

		if (ree_lseek(handle->fd, blknum * (sizeof(cfs_blk_t)), REE_SEEK_SET)
				!= 0) {
			fs_error("Failed to set file position!");
			goto cleanup;
		}

		if (ree_read(handle->fd, (void*) &blk, sizeof(cfs_blk_t))
				!= sizeof(cfs_blk_t)) {
			fs_error("Failed to read file data!");
			goto cleanup;
		}

		if (__fs_decrypt_and_verify(&blk, handle->key_data, 32, &data, blknum)
				!= 0) {
			fs_error("Failed to decrypt and verify file data!");
			goto cleanup;
		}

		uint32_t bdataLen = data.blk.count;

		if (bdataLen > sizeof(data.blk.blk)) {
			fs_error("Invalid data block detected!!");
			goto cleanup;
		}

		int32_t reading = bdataLen - blkoffset;

		if (reading < 0) {
			fs_error("Not enough data in this file");
			goto cleanup;
		}

		if (reading > left) {
			reading = left;
		}

		memcpy(curBuffer, &(data.blk.blk[blkoffset]), reading);

		curBuffer += reading;
		handle->filepos += reading;
		totalread += reading;
		left -= reading;

		if (bdataLen < sizeof(data.blk.blk)) {
			//this is the last block!
			if (totalread < count) {
				// not enough data here to read ...
				fs_error("Not enough data in this file");
				goto cleanup;
			}
		}

	} while (totalread < count);

	fs_debug("Read data:");
	kprintHex(buf, totalread);
	return (totalread);
	cleanup: handle->filepos = curFilepos;
	memset(buf, 0, totalread);
	return (-1);
}

int32_t fs_write(cfs_t* handle, void* buf, uint32_t count) {
	cfs_blk_t blk;
	cfs_data_t data;
	uint32_t curFilepos = handle->filepos;

	uint8_t* dataBuffer = (uint8_t*) data.blk.blk;
	uint8_t* curBuffer = buf;

	if (!__fs_check_init()) {
		fs_error("FS not initialized");
		return (-1);
	}

	fs_debug("Writting data:");
	kprintHex(buf, count);

	uint32_t left = count;
	// start reading at handle->filepos for count times
	uint32_t totalwritten = 0;
	uint32_t blknum = handle->filepos / CFS_BLK_SIZE;
	blknum = blknum + 1; // ignore first block
	uint32_t blkoffset = handle->filepos % CFS_BLK_SIZE;

	do {
		blknum = handle->filepos / CFS_BLK_SIZE;
		blknum = blknum + 1; // ignore first block
		blkoffset = handle->filepos % CFS_BLK_SIZE;
		fs_info("Writting blk #%d", blknum);

		// same as reading ....
		if (ree_lseek(handle->fd, blknum * (sizeof(cfs_blk_t)), REE_SEEK_SET)
				!= 0) {
			fs_error("Failed to set file position!");
			goto cleanup;
		}

		int32_t read_ret = ree_read(handle->fd, (void*) &blk,
				sizeof(cfs_blk_t));

		if (read_ret < 0) {
			fs_error("Failed to read file data!");
			goto cleanup;
		} else if (read_ret == sizeof(cfs_blk_t)) {
			// existing block ...
			if (__fs_decrypt_and_verify(&blk, handle->key_data, 32, &data,
					blknum) != 0) {
				fs_error("Failed to decrypt and verify file data!");
				goto cleanup;
			}
		} else if (read_ret == 0) {
			// new block
			fs_info("Setting data to zero!");
			memset(data.blk.blk, 0, sizeof(data.blk.blk));
			data.blk.count = 0;
		} else {
			fs_error("Invalid data block detected!!");
			goto cleanup;
		}

		// copy new data to block
		uint32_t bdataLen = data.blk.count;
		uint32_t writing = CFS_BLK_SIZE - blkoffset;
		if (writing > left) {
			writing = left;
		}
		fs_info(
				"Copy data to databuffer %d bytes 0x%x -> 0x%x", writing, curBuffer, dataBuffer+blkoffset);
		memcpy(dataBuffer + blkoffset, curBuffer, writing);
		data.blk.count = blkoffset + writing;
		bdataLen = data.blk.count;

		if (bdataLen > sizeof(data.blk.blk)) {
			fs_error("Invalid data block detected!!");
			goto cleanup;
		}

		if (__fs_encrypt_and_sign(&blk, handle->key_data, 32, &data, blknum)
				!= 0) {
			fs_error("Failed to encrypt and sign file data!");
			goto cleanup;
		}

		if (ree_lseek(handle->fd, blknum * (sizeof(cfs_blk_t)), REE_SEEK_SET)
				!= 0) {
			fs_error("Failed to set file position!");
			goto cleanup;
		}

		if (ree_write(handle->fd, (void*) &blk, sizeof(cfs_blk_t))
				!= sizeof(cfs_blk_t)) {
			fs_error("Failed to write block!");
			goto cleanup;
		}

		curBuffer += writing;
		handle->filepos += writing;
		totalwritten += writing;
		left -= writing;
	} while (totalwritten < count);

	return (totalwritten);
	cleanup: handle->filepos = curFilepos;
	return (-1);
}

int32_t fs_fstat(cfs_t* handle, fs_stat* buf) {
	fs_stat stat;
	cfs_blk_t blk;
	cfs_data_t data;

	if (!__fs_check_init()) {
		fs_error("FS not initialized");
		return (-1);
	}

	if (ree_fstat(handle->fd, &stat) != 0) {
		fs_error("Failed to stat file!");
		return (-1);
	}

	fs_info("State plain size: %d", stat.st_size);

	uint32_t blk_count = stat.st_size / sizeof(cfs_blk_t);
	--blk_count; //remove first block
	uint32_t size = blk_count * CFS_BLK_SIZE;
	if (size != 0) {

		++blk_count;

		// read last block to get data size ...
		if (ree_lseek(handle->fd, blk_count * (sizeof(cfs_blk_t)), REE_SEEK_SET)
				!= 0) {
			fs_error("Failed to set file position!");
			return (-1);
		}

		if (ree_read(handle->fd, (void*) &blk, sizeof(cfs_blk_t))
				!= sizeof(cfs_blk_t)) {
			fs_error("Failed to read file data!");
			return (-1);
		}

		if (__fs_decrypt_and_verify(&blk, handle->key_data, 32, &data,
				blk_count) != 0) {
			fs_error("Failed to decrypt and verify file data!");
			return (-1);
		}

		uint32_t bdataLen = data.blk.count;
		size = size - CFS_BLK_SIZE + bdataLen;
	}

	buf->st_size = size;
	return (0);
}

uint32_t fs_lseek(cfs_t* handle, uint32_t offset, int32_t whence) {
	uint32_t ret = ree_lseek(handle->fd, offset, whence);
	if (ret != (uint32_t) -1) {
		switch (whence) {
		case REE_SEEK_CUR:
			handle->filepos += offset;
			break;
		case REE_SEEK_SET:
			handle->filepos = offset;
			break;
		default:
			fs_error("Invalid whence value!");
			break;
		}
	}
	return (ret);
}

