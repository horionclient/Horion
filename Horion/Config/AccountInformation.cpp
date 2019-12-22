#include "AccountInformation.h"

AccountInformation::AccountInformation(std::string discordTok, unsigned int serial) : discordToken(discordTok), isGuest(false), serialNum(serial) {};
AccountInformation::AccountInformation() : discordToken("none"), isGuest(true) {};

bool AccountInformation::verify() {
	if (isGuest)
		return true;
	if (discordToken.size() < 10)
		return false;
	if (didVerify)
		return isValid;
	didVerify = true;

	wchar_t fullUrl[200];
	swprintf_s(fullUrl, 200, L"http://www.horionbeta.club:50451/api/beta/check?client=%S&serial=%u", discordToken.c_str(), serialNum);
	WinHttpClient client(fullUrl);
	client.SetTimeouts(1500, 3000, 2000, 3000);
	bool httpRequest = client.SendHttpRequest();
	if (!httpRequest) {
		logF("Account verification failed, %d", client.GetLastError());
		return false;
	}

	// The response header.
	std::wstring status = client.GetResponseStatusCode();
	if (status == L"200") {
		std::wstring content = client.GetResponseContent();
		nlohmann::json data = nlohmann::json::parse(content);

		if (data.contains("status") && data["status"].is_string() && data["status"].get<std::string>() == "success" || data["status"].get<std::string>() == "nobeta") {
			logF("Account verified");
			isValid = true;
			return true;
		}
	}

	return false;
};

AccountInformation AccountInformation::fromToken(std::string discordToken, unsigned int serial) {
	auto acc = AccountInformation(discordToken, serial);
	return acc;
}

AccountInformation AccountInformation::asGuest() {
	return AccountInformation();
}
