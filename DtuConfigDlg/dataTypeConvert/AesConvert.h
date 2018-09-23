#pragma once
#include "openssl/aes.h"
#include "include/mty.h"
extern bool gEncryptAes;

int encry_aes(MU_INT8* input_data, MU_INT32 input_data_len);
int decrypt_aes(MU_INT8* input_data, MU_INT32 input_data_len);
