#include "../include/aes.h"  
 
#include <string.h>  
#include <stdlib.h>
#include <openssl/aes.h>


int aes_encrypt(unsigned char* in, unsigned char** out, char *key, int inlen, int *outlen, int type)
{
	AES_KEY aes;
    unsigned char iv[AES_BLOCK_SIZE];        // init vector
	unsigned char* input_string;
	unsigned char* output_string;
	unsigned int len;        // encrypt length (in multiple of AES_BLOCK_SIZE)
	
	// set the encryption length
	input_string = NULL;
	output_string = NULL;
    len = 0;

    memcpy(iv, key, strlen(key)+1);

    if (inlen % AES_BLOCK_SIZE == 0) {
        len = inlen;
    } else {
        len = (inlen / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }
	

	// set the input string
    input_string = (unsigned char*)calloc(len, sizeof(unsigned char));
    if (input_string == NULL) {
        goto failed;
    }
	memcpy(input_string, in, inlen);

	if (AES_set_encrypt_key((unsigned char *)key, 128, &aes) < 0) {
		goto failed;
	}
	
	// alloc encrypt_string
    output_string = (unsigned char*)calloc(len, sizeof(unsigned char));
    if (output_string == NULL) {
        goto failed;
    }

    // encrypt (iv will change)
	if (type == 0) {
		AES_cbc_encrypt(input_string, output_string, len, &aes, iv, AES_ENCRYPT);
	} else {
		AES_cbc_encrypt(input_string, output_string, len, &aes, iv, AES_DECRYPT);
	}
	*outlen = len;
	*out = output_string;
	
	free(input_string);
	return 0;

failed:
	if (input_string) {
		free(input_string);
	}
	
	if (output_string) {
		free(output_string);
	}
	
    return -1;
}

