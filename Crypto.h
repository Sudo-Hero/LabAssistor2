#ifndef _CRYPTO_
#define _CRYPTO_

#include <iostream>
#include <windows.h>
#include <string>
#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"

#include <iostream>
#include <string>

using namespace CryptoPP;


// Define your fixed key (128, 192, or 256 bits, depending on your needs)
byte fixedKey[AES::DEFAULT_KEYLENGTH] = { 0xF0, 0xD1, 0x05, 0x09, 0xF4, 0xA1, 0xA0, 0x07, 0x12, 0x15, 0x01, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
SecByteBlock key(fixedKey, sizeof(fixedKey));

// Define your fixed IV (128 bits)
byte fixedIV[AES::BLOCKSIZE] = { 0x00, 0xAA, 0xAF, 0x1E, 0x26, 0x15, 0x16, 0x17, 0xFF, 0x12, 0x1A, 0xBB, 0x1C, 0x1D, 0x1E, 0x1F };
SecByteBlock iv(fixedIV, sizeof(fixedIV));


std::string encrypt(SecByteBlock& key, SecByteBlock& iv, std::string& plain);

#endif // !_CRYPTO_

