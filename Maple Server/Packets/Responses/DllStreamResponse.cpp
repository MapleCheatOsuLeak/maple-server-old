#include "DllStreamResponse.h"

#include "../../Globals.h"
#include "../../Utils/StringUtilities.h"

DllStreamResponse::DllStreamResponse(int cheatID, MatchedClient* matchedClient) : Response(ResponseType::DllStream)
{
	std::vector<unsigned char> response;
	if (matchedClient->Subscriptions[cheatID])
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
		AddByte(static_cast<unsigned char>(DllStreamResult::NotSubscribed), &response);

		AddString(matchedClient->AES.Encrypt(response));
	}
}
