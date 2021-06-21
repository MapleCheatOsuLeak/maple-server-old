#pragma once
#include "Crypto/Wrapper/RSAEncrypt.h"

class Globals
{
public:
	static inline RSAEncrypt* RSA = new RSAEncrypt();
};
