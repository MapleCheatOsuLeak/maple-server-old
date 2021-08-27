#include "LoginResponse.h"

#include <curl.h>
#include <json.hpp>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
	return size * nmemb;
}

LoginResponse::LoginResponse(std::string hwid, std::string hash, std::string username, std::string password,
                             MatchedClient* matchedClient) : Response(ResponseType::Login)
{
	std::vector<unsigned char> loginInfo;

	try
	{
		CURL* curl = curl_easy_init();
		if (curl)
		{
			std::string readBuffer;

			curl_easy_setopt(curl, CURLOPT_URL, "https://maple.software/backend/auth");
			std::string postString = "t=0&u=" + username + "&p=" + password + "&h=" + hwid + "&ha=" + hash;
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

				AddByte(static_cast<unsigned char>(LoginResult::InternalError), &loginInfo);
				AddString(matchedClient->AES.Encrypt(loginInfo));
			}
			else
			{
				curl_easy_cleanup(curl);

				auto response = nlohmann::json::parse(readBuffer);
				int code = response.at("code");
				if (code == 0)
				{
					std::string sessionID = response.at("sessionID");

					AddByte(static_cast<unsigned char>(LoginResult::Success), &loginInfo);
					AddString(sessionID, &loginInfo);
					AddString(matchedClient->AES.Encrypt(loginInfo));

					std::vector<unsigned char> games;
					for (auto& arr : response["games"].items())
					{
						auto game = arr.value();
						std::vector<unsigned char> gameString;
						
						int id = game["ID"];
						std::string name = game["Name"];
						std::string moduleName = game["ModuleName"];
						
						AddString(std::to_string(id), &gameString);
						AddString(name, &gameString);
						AddString(moduleName, &gameString);

						AddString(matchedClient->AES.Encrypt(gameString), &games);
					}

					matchedClient->Subscriptions.clear();
					
					std::vector<unsigned char> cheats;
					for (auto& arr : response["cheats"].items())
					{
						auto cheat = arr.value();
						std::vector<unsigned char> cheatString;

						int id = cheat["ID"];
						int gameID = cheat["GameID"];
						std::string releaseStreams = cheat["ReleaseStreams"];
						std::string name = cheat["Name"];
						int price = cheat["Price"];
						int status = cheat["Status"];
						std::string features = cheat["Features"];
						std::string expiresAt = cheat["ExpiresAt"];

						AddString(std::to_string(id), &cheatString);
						AddString(std::to_string(gameID), &cheatString);
						AddString(releaseStreams, &cheatString);
						AddString(name, &cheatString);
						AddString(std::to_string(price), &cheatString);
						AddString(std::to_string(status), &cheatString);
						AddString(features, &cheatString);
						AddString(expiresAt, &cheatString);

						AddString(matchedClient->AES.Encrypt(cheatString), &cheats);

						matchedClient->Subscriptions.insert({ id, expiresAt != "not subscribed" });
					}

					AddString(matchedClient->AES.Encrypt(games));
					AddString(matchedClient->AES.Encrypt(cheats));

					// Set values in MatchedClient
					matchedClient->HWID = hwid;
					matchedClient->SessionToken = sessionID;
				}
				else
				{
					LoginResult result;
					switch (code)
					{
						case 1:
							result = LoginResult::IncorrectCredentials;
							break;
						case 2:
							result = LoginResult::HashMismatch;
							break;
						case 3:
							result = LoginResult::HWIDMismatch;
							break;
						case 4:
							result = LoginResult::Banned;
							break;
						default:
							result = LoginResult::InternalError;
					}

					AddByte(static_cast<unsigned char>(result), &loginInfo);
					AddString(matchedClient->AES.Encrypt(loginInfo));
				}
			}
		}
		else
		{
			AddByte(static_cast<unsigned char>(LoginResult::InternalError), &loginInfo);
			AddString(matchedClient->AES.Encrypt(loginInfo));
		}
	}
	catch(...)
	{
		AddByte(static_cast<unsigned char>(LoginResult::InternalError), &loginInfo);
		AddString(matchedClient->AES.Encrypt(loginInfo));
	}
}
