#include "AES.h"

AESWrapper::AESWrapper()
{
}

void AESWrapper::GenerateRandomIV()
{
	for (int i = 0; i < 16; i++) {
		unsigned char byte = rand() % 256;
		IV.push_back(byte);
	}

	/*genBytes gB;
	std::generate(AES::IV.begin(), AES::IV.end(), std::ref(gB));*/
}

void AESWrapper::GenerateRandomKey()
{
	for (int i = 0; i < 32; i++) {
		unsigned char byte = rand() % 256;
		Key.push_back(byte);
	}
	/*genBytes gB;
	std::generate(AES::Key.begin(), AES::Key.end(), std::ref(gB));*/
}

std::vector<unsigned char> AESWrapper::DumpIV()
{
	return AESWrapper::IV;
}

std::vector<unsigned char> AESWrapper::DumpKey()
{
	return AESWrapper::Key;
}

std::string AESWrapper::Encrypt(std::string input)
{
	using namespace CryptoPP;

	if (AESWrapper::IV.empty() || AESWrapper::IV.size() <= 0)
		return "IV null";
	if (AESWrapper::Key.empty() || AESWrapper::Key.size() <= 0)
		return "Key null";

	AutoSeededRandomPool prng;
	HexEncoder encoder(new FileSink(std::cout));

	SecByteBlock key(reinterpret_cast<const byte*>(&AESWrapper::Key[0]), AESWrapper::Key.size());
	SecByteBlock iv(reinterpret_cast<const byte*>(&AESWrapper::IV[0]), AESWrapper::IV.size());

	std::string cipher;

	try
	{
		CBC_Mode< AES >::Encryption e;
		e.SetKeyWithIV(key, key.size(), iv);

		StringSource s(input, true,
			new StreamTransformationFilter(e,
				new StringSink(cipher)
			)
		);
	}
	catch (CryptoPP::Exception & e)
	{
		return e.what();
	}

	return cipher;
}

std::string AESWrapper::Decrypt(std::string input)
{
	using namespace CryptoPP;
	if (AESWrapper::IV.empty() || AESWrapper::IV.size() <= 0)
		return "IV null";
	if (AESWrapper::Key.empty() || AESWrapper::Key.size() <= 0)
		return "Key null";

	AutoSeededRandomPool prng;
	HexEncoder encoder(new FileSink(std::cout));

	SecByteBlock key(reinterpret_cast<const byte*>(&AESWrapper::Key[0]), AESWrapper::Key.size());
	SecByteBlock iv(reinterpret_cast<const byte*>(&AESWrapper::IV[0]), AESWrapper::IV.size());

	std::string recovered;

	try
	{
		CBC_Mode< AES >::Decryption d;
		d.SetKeyWithIV(key, key.size(), iv);

		StringSource s(input, true,
			new StreamTransformationFilter(d,
				new StringSink(recovered)
			)
		);

	}
	catch (CryptoPP::Exception & e)
	{
		return e.what();
	}

	return recovered;
}
