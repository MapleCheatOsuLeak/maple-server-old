#include "HeartbeatResponse.h"

#include <json.hpp>
#include <chrono>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
	return size * nmemb;
}

HeartbeatResponse::HeartbeatResponse(std::string sessionID, MatchedClient* matchedClient) : Response(ResponseType::Heartbeat)
{
	std::vector<unsigned char> response;
	try
	{
		CURL* curl = curl_easy_init();
		if (curl)
		{
			std::string readBuffer;

			curl_easy_setopt(curl, CURLOPT_URL, "https://maple.software/backend/auth");
			std::string postString = "t=1&e=1&s=" + sessionID;
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

				AddByte(static_cast<unsigned char>(HeartbeatResult::InternalError), &response);
				AddString(matchedClient->AES.Encrypt(response));
			}
			else
			{
				curl_easy_cleanup(curl);

				auto jsonResponse = nlohmann::json::parse(readBuffer);
				int code = jsonResponse.at("code");
				HeartbeatResult result;
				switch (code)
				{
					case 0:
						result = HeartbeatResult::Success;
						break;
					case 4:
						result = HeartbeatResult::InvalidSession;
						break;
					default:
						result = HeartbeatResult::InternalError;
						break;
				}

				std::chrono::milliseconds msEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
				AddByte(static_cast<unsigned char>(result), &response);
				AddString(std::to_string((msEpoch.count() / 2) ^ 0xDA));
				AddString(matchedClient->AES.Encrypt(response));
			}
		}
		else
		{
			AddByte(static_cast<unsigned char>(HeartbeatResult::InternalError), &response);
			AddString(matchedClient->AES.Encrypt(response));
		}
	}
	catch (...)
	{
		AddByte(static_cast<unsigned char>(HeartbeatResult::InternalError), &response);
		AddString(matchedClient->AES.Encrypt(response));
	}
}
