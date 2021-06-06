#ifndef RSAENCRYPT_H
#define RSAENCRYPT_H

#include <string>
#include "../base64.h"
#include "../rsa.h"
#include "../osrng.h"
#include "../hex.h"
#include "../files.h"
#include "../pssr.h"

using namespace CryptoPP;

class RSAEncrypt
{
private:
	std::string privKey;
	std::string privKeyDecoded;

	void decodePrivKeyBase64();
public:
	RSAEncrypt();

	std::string Encrypt(std::string input);
};

#endif