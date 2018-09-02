#pragma once

#include "CmFunction/CmDefStruct.h"

#include "openssl/openssl/aes.h"


namespace NSP_STP_CM
{
	int encry_aes(MU_INT8* input_data, MU_INT32 input_data_len);
	int decrypt_aes(MU_INT8* input_data, MU_INT32 input_data_len);
}