#pragma once

#include <vector>
#include <string.h>
#include <stdlib.h>
#include <regex>

static std::vector<std::string> Split(std::string input, char delimiter)
{
	std::vector<std::string> ret = std::vector<std::string>();
	int init_size = strlen(input.c_str());

	char* ptr = strtok(strdup(input.c_str()), &delimiter);

	while (ptr != NULL)
	{
		ret.push_back(ptr);
		ptr = strtok(NULL, &delimiter);
	}
	return ret;
}