#include "AesConvert.h"
#include "assert.h"
#include "string.h"
#include "Log.h"

bool gEncryptAes = false;

int encry_aes(MU_INT8* input_data, MU_INT32 input_data_len)
{
    AES_KEY aes;
    MU_INT8 key[AES_BLOCK_SIZE];        // AES_BLOCK_SIZE = 16
    MU_INT8 iv[AES_BLOCK_SIZE];        // init vector

    if (!gEncryptAes)
    {
        return 0;
    }

    if((input_data_len % AES_BLOCK_SIZE ) != 0)
    {
        gLog.Add("%s input_data_len % AES_BLOCK_SIZE != 0", __FUNCTION__);
        return -1;
    }

    // Generate AES 128-bit key
    for (MU_INT32 i = 0; i < AES_BLOCK_SIZE; ++i)
    {
        key[i] = i + 1;
    }

    // Set encryption key
    for (MU_INT32 i = 0; i < AES_BLOCK_SIZE; ++i)
    {
        iv[i] = i + 1;
    }

    if (AES_set_encrypt_key(key, 128, &aes) < 0)
    {
        return -1;
    }

    MU_INT8* encrypt_data = new MU_INT8[input_data_len];
    if (encrypt_data == NULL)
    {
        return -1;
    }
    // encrypt (iv will change)
    AES_cbc_encrypt(input_data, encrypt_data, input_data_len, &aes, iv, AES_ENCRYPT);

    memcpy(input_data, encrypt_data, input_data_len);
    delete[] encrypt_data;

    // print
    //    printf("\n encrypt_data = ");
    //	for (MU_INT32 i = 0; i < input_data_len; ++i)
    //	{
    //        printf("%02x ", input_data[i]);
    //	}

    return 0;
}

int decrypt_aes(MU_INT8* input_data, MU_INT32 input_data_len)
{
    AES_KEY aes;
    MU_INT8 key[AES_BLOCK_SIZE];        // AES_BLOCK_SIZE = 16
    MU_INT8 iv[AES_BLOCK_SIZE];        // init vector

    if (!gEncryptAes)
    {
        return 0;
    }

    if((input_data_len % AES_BLOCK_SIZE ) != 0)
    {
        gLog.Add("%s input_data_len % AES_BLOCK_SIZE != 0", __FUNCTION__);
        return -1;
    }

    // Generate AES 128-bit key
    for (MU_INT32 i = 0; i < AES_BLOCK_SIZE; ++i)
    {
        key[i] = i + 1;
    }

    // Set decryption key
    for (MU_INT32 i = 0; i < AES_BLOCK_SIZE; ++i)
    {
        iv[i] = i + 1;
    }

    if (AES_set_decrypt_key(key, 128, &aes) < 0)
    {
        return -1;
    }

    // alloc decrypt_string
    MU_INT8* decrypt_data = new unsigned char[input_data_len];
    if (decrypt_data == NULL)
    {
        return -1;
    }
    // decrypt
    AES_cbc_encrypt(input_data, decrypt_data, input_data_len, &aes, iv, AES_DECRYPT);
    memcpy(input_data, decrypt_data, input_data_len);
    delete[] decrypt_data;
    // print

    //	printf("\n decrypt_data = ");
    //	for (MU_INT32 i = 0; i < input_data_len; ++i)
    //	{
    //		printf("%02x ", input_data[i]);
    //	}
    return 0;
}
