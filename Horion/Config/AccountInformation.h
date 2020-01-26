#pragma once

#include <string>

#include "../../Utils/Logger.h"

class AccountInformation {
private:
	bool isGuest;
	bool didVerify = false;  // Did check if its verified?
	bool isValid = false;    // Is it a verified account
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
