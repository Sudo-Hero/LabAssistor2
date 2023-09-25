#include "DES.h"
#include <iostream>
#include <string>
#include <sstream>
#include "string.h"
const char* HexaDigits = "0123456789abcdef";

void trimCharArray(const char input[], int numBytesToRemove, char output[]) {
	int length = strlen(input);

	// Ensure numBytesToRemove is within valid bounds
	if (numBytesToRemove < 0) {
		numBytesToRemove = 0;
	}
	else if (numBytesToRemove > length) {
		numBytesToRemove = length;
	}

	// Calculate the number of bytes to keep
	int numBytesToKeep = length - numBytesToRemove;

	// Copy the desired portion of the original array to the output array using strcpy_s
	strcpy_s(output, numBytesToKeep + 1, input + numBytesToRemove);
}

char* decrypt(char* data) {
	try {
		char key[8] = { 0xAA, 0xBB, 0x09, 0x18, 0x27, 0x36, 0xCC, 0xDD };
		char cipherText[256] = { 0 };
		std::string sdata(data);
		strncpy_s(cipherText, 256, sdata.c_str(), 256);

		// Round Keys for DES Encryption and Decryption
		char roundKeys[16][8];

		char original[9] = { 0 };//Orignal text
		std::string text;
		//PLAINTEXT
		int count = 0;
		for (int i = 0; strnlen_s(cipherText, 19) != 0; i++) {
			char temp[9];
			memset(temp, 0, 9);
			strncpy_s(temp, cipherText, 8);
			trimCharArray(cipherText, 8, cipherText);
			memset(original, 0, 8);
			//strncpy(temp, plainText, count);
			DES_CreateKeys(key, roundKeys);
			DES_Decrypt(temp, roundKeys, original);

			text = text + std::string(original);
		}
		std::cout << std::endl << text << " " << text.length() << std::endl;
		char* pttext = new char[20];
		strncpy_s(pttext, 20, text.c_str(), 20);
		pttext[19] = '\0';
		return pttext;
	}
	catch (std::exception e) {
		std::cout << e.what();
	}

}

//ASD2-ASDF-66SF-56SF
char* encrypt(const char* ctdata){
	try {
		std::string cdata(ctdata);
		if (cdata.length() != 19) {
			return nullptr;
		}
		char data[20];
		strncpy_s(data, 20, cdata.c_str(), 20);
		// Input values to the encryption
		char key[8] = { 0xAA, 0xBB, 0x09, 0x18, 0x27, 0x36, 0xCC, 0xDD };
		char plainText[20];
		strncpy_s(plainText, data, 19);

		// Round Keys for DES Encryption and Decryption
		char roundKeys[16][8];
		// Ciphertext returned after encryption, MUST be initialized to 0s
		char cipherText[9] = { 0 };
		// Plaintext from decrypting the ciphertext, MUST be initialized to 0s
		std::string cipher;
		int count = 0;
		for (int i = 0; strnlen_s(plainText, 19) != 0; i++) {
			char temp[8];
			strncpy_s(temp, plainText, 7);
			trimCharArray(plainText, 7, plainText);
			memset(cipherText, 0, 9);
			//strncpy(temp, plainText, count);
			DES_CreateKeys(key, roundKeys);
			DES_Encrypt(temp, roundKeys, cipherText);
			cipherText[8] = '\0';
			cipher = cipher + cipherText;
		}
		char* ctext = new char[256];
		strncpy_s(ctext, 256, cipher.c_str(), 256);
		ctext[cipher.length()] = '\0';
		//print values test
		//std::cout << std::endl << cipher << " " << cipher.length() << std::endl;
		//std::cout << std::endl << text << " " << text.length() << std::endl;
		return ctext;
	}
	catch (std::exception e) {
		std::cout << e.what();
		return nullptr;
	}

}
