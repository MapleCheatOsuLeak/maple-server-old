#include "AES.h"

AESWrapper::AESWrapper()
{
	srand(static_cast<unsigned>(time(nullptr)));  // NOLINT(cert-msc51-cpp)
	GenerateRandomIV();
	GenerateRandomKey();
}

void AESWrapper::GenerateRandomIV()
{
	for (int i = 0; i < 16; i++)
		IV[i] = rand() % 256;// NOLINT(clang-diagnostic-implicit-int-conversion)
}

void AESWrapper::GenerateRandomKey()
{
	for (int i = 0; i < 32; i++) 
		Key[i] = rand() % 256; // NOLINT(clang-diagnostic-implicit-int-conversion)
}

std::vector<unsigned char> AESWrapper::DumpIV()
{
	return IV;
}

std::vector<unsigned char> AESWrapper::DumpKey()
{
	return Key;
}

std::string AESWrapper::Encrypt(std::vector<unsigned char> input)
{
	using namespace CryptoPP;

	if (IV.empty() || IV.size() <= 0)
		return "IV null";
	if (Key.empty() || Key.size() <= 0)
		return "Key null";

	AutoSeededRandomPool prng;
	HexEncoder encoder(new FileSink(std::cout));

	SecByteBlock key((&Key[0]), Key.size());
	SecByteBlock iv((&IV[0]), IV.size());

	std::string cipher;

	try
	{
		CBC_Mode<AES>::Encryption e;
		e.SetKeyWithIV(key, key.size(), iv);

		VectorSource s(input, true,
		               new StreamTransformationFilter(e,
		                                              new StringSink(cipher)
		               )
		);
	}
	catch (Exception& e)
	{
		return e.what();
	}

	return cipher;
}

std::vector<unsigned char> AESWrapper::Decrypt(std::vector<unsigned char> input)
{
	using namespace CryptoPP;
	if (IV.empty() || IV.size() <= 0)
		return std::vector<unsigned char>();
	if (Key.empty() || Key.size() <= 0)
		return std::vector<unsigned char>();

	AutoSeededRandomPool prng;
	HexEncoder encoder(new FileSink(std::cout));

	SecByteBlock key((&Key[0]), Key.size());
	SecByteBlock iv((&IV[0]), IV.size());

	std::string recovered;
	std::vector<unsigned char> decrypt = std::vector<unsigned char>();

	CryptoPP::AES::Decryption aesDecryption(key, 32);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::VectorSink(decrypt));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(&input[0]), input.size());
	stfDecryptor.MessageEnd();

	try
	{
		CBC_Mode<AES>::Decryption d;
		d.SetKeyWithIV(key, key.size(), iv);

		VectorSource s(input, true,
		               new StreamTransformationFilter(d,
		                                              new StringSink(recovered)
		               )
		);
	}
	catch (Exception& e)
	{
		return std::vector<unsigned char>();
	}

	return decrypt;
}
