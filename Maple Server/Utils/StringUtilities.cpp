#pragma once

#include <vector>
#include <string.h>
#include <stdlib.h>
#include <regex>

static std::vector<std::string> Split(const std::string str)
{
	const std::regex re(R"(0xdeadbeef)");
	const std::sregex_token_iterator it{
		str.begin(),
		str.end(), re, -1
	};
	std::vector<std::string> tokenized{ it, {} };

	tokenized.erase(
		std::remove_if(tokenized.begin(),
			tokenized.end(),
			[](const std::string & s)
			{
				return s.size() == 0;
			}),
		tokenized.end());

	return tokenized;
}

static std::vector<std::string> Split(std::string input, char delimiter)
{
	std::vector<std::string> ret = std::vector<std::string>();
	int init_size = strlen(input.c_str());

	char* ptr = strtok(_strdup(input.c_str()), &delimiter);

	while (ptr != nullptr)
	{
		// Copy the element, instead of pushing the reference.
		// pushing the reference can lead to undefined behavior, if destruction happens before construction
		// bug #0001
		ret.emplace_back(ptr);
		ptr = strtok(nullptr, &delimiter);
	}
	return ret;
}