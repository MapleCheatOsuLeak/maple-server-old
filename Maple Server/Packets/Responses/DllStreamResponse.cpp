#include "DllStreamResponse.h"

#include <json.hpp>

#include "../../Globals.h"
#include "../../Utils/StringUtilities.h"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
	return size * nmemb;
}

DllStreamResponse::DllStreamResponse(int cheatID, MatchedClient* matchedClient) : Response(ResponseType::DllStream)
{
	std::vector<unsigned char> response;
	if (matchedClient->Subscriptions[cheatID])
	{
		try
		{
			CURL* curl = curl_easy_init();
			if (curl)
			{
				std::string readBuffer;

				curl_easy_setopt(curl, CURLOPT_URL, "https://maple.software/backend/auth");
				std::string postString = "t=1&e=0&s=" + matchedClient->SessionToken;
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postString.c_str());
				curl_easy_setopt(curl, CURLOPT_USERAGENT, "mapleserver/azuki is a cutie");
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
				CURLcode res = curl_easy_perform(curl);
				if (res != CURLE_OK)
				{
					fprintf(stderr, "curl_easy_perform() failed: %s\n",
						curl_easy_strerror(res));

					curl_easy_cleanup(curl);

					AddByte(static_cast<unsigned char>(DllStreamResult::InternalError), &response);
					AddString(matchedClient->AES.Encrypt(response));
				}
				else
				{
					curl_easy_cleanup(curl);

					auto jsonResponse = nlohmann::json::parse(readBuffer);
					int code = jsonResponse.at("code");
					if (code == 0)
					{
						// user has active maple subscription
						std::string file = "C:\\Cheats\\" + std::to_string(cheatID) + ".dll";

						std::ifstream inFile(file, std::ios_base::binary);

						inFile.seekg(0, std::ios_base::end);
						size_t length = inFile.tellg();
						inFile.seekg(0, std::ios_base::beg);

						std::vector<unsigned char> buffer;
						buffer.reserve(length);
						std::copy(std::istreambuf_iterator<char>(inFile),
							std::istreambuf_iterator<char>(),
							std::back_inserter(buffer));

						AddByte(static_cast<unsigned char>(DllStreamResult::Success), &response);
						AddString(buffer, &response);

						AddString(matchedClient->AES.Encrypt(response));
					}
					else
					{
						if (code == 4)
						{
							matchedClient->HWID.clear();
							matchedClient->SessionToken.clear();
							matchedClient->SessionToken.clear();
						}
						
						AddByte(static_cast<unsigned char>(code == 4 ? DllStreamResult::InvalidSession : DllStreamResult::InternalError), &response);
						AddString(matchedClient->AES.Encrypt(response));
					}
				}
			}
			else
			{
				AddByte(static_cast<unsigned char>(DllStreamResult::InternalError), &response);
				AddString(matchedClient->AES.Encrypt(response));
			}
		}
		catch (...)
		{
			AddByte(static_cast<unsigned char>(DllStreamResult::InternalError), &response);
			AddString(matchedClient->AES.Encrypt(response));
		}
	}
	else
	{
		AddByte(static_cast<unsigned char>(DllStreamResult::NotSubscribed), &response);

		AddString(matchedClient->AES.Encrypt(response));
	}
}
