#include "LoginResponse.h"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
	return size * nmemb;
}

LoginResponse::LoginResponse(std::string hwid, std::string username, std::string password,
                             MatchedClient* matchedClient) : Response(ResponseType::Login)
{
	unsigned char loginResult = 0x0; // 0x0 - success, 0x1 - incorrect credentials, 0x2 - hwid mismatch, 0x3 wrong shit?

	std::string sessionToken("qwertyuiop");
	std::string expiresAt("Never");

	std::vector<unsigned char> loginInfo;
	std::cout << hwid << " | " << username << " | " << password << std::endl;

	CURL* curl = curl_easy_init();
	if (curl)
	{
		std::string readBuffer;

		curl_easy_setopt(curl, CURLOPT_URL, "https://maple.software/backend/auth");
		std::string postString = "t=0&u=" + username + "&p=" + password + "&h=" + hwid;
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postString.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "mapleserver/azuki is a cutie");
		std::cout << postString.c_str() << std::endl;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			        curl_easy_strerror(res));

		curl_easy_cleanup(curl);
		std::cout << readBuffer << std::endl;

		//code=00xdeadbeefsessionID=a60cff41d8fa0ee48def0bfb84fbdd220xdeadbeefexpiresAt=not subscribed -> resp
		if (readBuffer == "")
			loginResult = 0x3;
		else
		{
			std::vector<std::string> split = StringUtilities::Split(readBuffer);

			std::vector<std::string> code = StringUtilities::Split(split[0], "(=)");
			if (code[0] != "code")
				loginResult = 0x3;
			else if (code[1] == "0")
			{
				std::vector<std::string> sessionId = StringUtilities::Split(split[1], "(=)");
				std::vector<std::string> expiresat = StringUtilities::Split(split[2], "(=)");

				sessionToken = sessionId[1];
				expiresAt = expiresat[1];
			}
			else if (code[1] == "1")
				loginResult = 0x1;
			else if (code[1] == "2")
				loginResult = 0x2;
		}
	}


	AddByte(loginResult, &loginInfo);
	AddString(sessionToken, &loginInfo);
	AddString(expiresAt, &loginInfo);

	AddString(matchedClient->AES.Encrypt(loginInfo));
	// Set values in MatchedClient
	matchedClient->ExpiresAt = expiresAt;
	matchedClient->HWID = hwid;
	matchedClient->SessionToken = sessionToken;
}
