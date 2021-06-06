#include "Login.h"

struct MemoryStruct {
	char* memory;
	size_t size;
};

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	void* ptr = realloc(mem->memory, mem->size + realsize + 1);
	if (!ptr) {
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = (char*)ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

static LoginPacket ConstructLoginPacket(const char* msg)
{
	std::vector<std::string> split = Split(msg, '|');
	std::string hwid = split[0];
	std::string username = split[1];
	std::string password = split[2];
	std::string ip = split[3];
	return LoginPacket(hwid, username, password, ip);
}

static std::string HandleLogin(const Client& client, LoginPacket lp)
{
	bool ok = false;
	CURL* curl;
	CURLcode res;
	struct MemoryStruct chunk;
	std::string packet = "type=l&hwid=" + lp.hwid + "&username=" + lp.username + "&password=" + lp.password + "&ip=" + client.getIp();
	static const char* postthis = packet.c_str();

	chunk.memory = (char*)malloc(1);  /* will be grown as needed by realloc above */
	chunk.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://maple.software/backend/MapleServer/Request.php");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)& chunk);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}
		else {
			/*
			 * Now, our chunk.memory points to a memory block that is chunk.size
			 * bytes big and contains the remote file.
			 *
			 * Do something nice with it!
			 */
			ok = true;
			printf("%s\n", chunk.memory);
		}

		curl_easy_cleanup(curl);
	}

	free(chunk.memory);
	curl_global_cleanup();
	return (ok ? "OK" : "NOT OK");
}