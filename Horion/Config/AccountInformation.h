#pragma once

#include "../../Utils/Logger.h"
#include "../../include/WinHttpClient.h"
#include "../../Utils/Json.hpp"
#include <string>

class AccountInformation
{
private:
	bool isGuest;
	std::string name;
	std::string discordToken;
	unsigned int serialNum = 0;

	AccountInformation(std::string, unsigned int);
	AccountInformation();
public:
	bool verify();

	static AccountInformation fromToken(std::string discordToken, unsigned int serial);
	static AccountInformation asGuest();
};

