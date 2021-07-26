#include "DllStreamResponse.h"

#include "../../Globals.h"
#include "../../Utils/StringUtilities.h"

DllStreamResponse::DllStreamResponse(std::string product, MatchedClient* matchedClient) : Response(ResponseType::DllStream)
{
	if (matchedClient->ExpiresAt != "not subscribed")
	{
		// user has active maple subscription
		std::string file = "";
		if (product == "osu-lite")
			file = "C:\\maple-lite.dll";
		else if (product == "osu-full")
			file = "C:\\maple-full.dll";
		
		std::ifstream inFile(file, std::ios_base::binary);

		inFile.seekg(0, std::ios_base::end);
		size_t length = inFile.tellg();
		inFile.seekg(0, std::ios_base::beg);

		std::vector<unsigned char> buffer;
		buffer.reserve(length);
		std::copy(std::istreambuf_iterator<char>(inFile),
			std::istreambuf_iterator<char>(),
			std::back_inserter(buffer));

		std::string s = matchedClient->AES.Encrypt(buffer);
		AddString(s);
	}
}
