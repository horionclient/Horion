#include "AccountInformation.h"

#include "../../Utils/Json.hpp"
#include "../../include/WinHttpClient.h"
#include "../../Memory/GameData.h"
#include "../../Utils/Logger.h"

AccountInformation::AccountInformation(std::string authTok, unsigned int serial) : isGuest(false), authToken(authTok), serialNum(serial){};
AccountInformation::AccountInformation() : isGuest(true), authToken("none"){};

bool AccountInformation::verify() {
	if (isGuest)
		return true;
	if (authToken.size() < 10)
		return false;
	if (didVerify)
		return isValid;
	didVerify = true;

	wchar_t formatString[130]; // Little hack for xor'd url
	swprintf_s(formatString, 130, L"%S", XorString("http://www.horionbeta.club/api/beta/check?client=%S&serial=%u&edition=%S&compile=%S&stamp=%u"));
	
	wchar_t fullUrl[250];

#ifdef _DEBUG
	const char* edition = "dev";
#elif defined(_BETA)
	const char* edition = "beta";
#else
	const char* edition = "public";
#endif

	swprintf_s(fullUrl, 250, formatString, authToken.c_str(), serialNum, edition, XorString(__TIME__), g_Data.networkedData.xorKey);
	WinHttpClient client(fullUrl);
	client.SetTimeouts(1500, 3000, 2000, 3000);
	bool boi = client.SendHttpRequest();
	if (!boi) {
		logF("Account verification failed, %d", client.GetLastError());
		return false;
	}

	// The response header.
	std::wstring status = client.GetResponseStatusCode();
	if (status == L"200") {
		std::wstring content = client.GetResponseContent();
		nlohmann::json data = nlohmann::json::parse(content);

		if (data.contains("status") && data["status"].is_string() && data["status"].get<std::string>() == "success") {
			logF("Account verified");

			// evade and deceive
			g_Data.networkedData.localPlayerOffset = data["serverTime"].get<int>(); 
			g_Data.networkedData.dataSet = true;

			isValid = true;
			return true;
		} else
			logF("Account is a guest account");
	}

	return false;
};

AccountInformation AccountInformation::fromToken(std::string authToken, unsigned int serial) {
	auto acc = AccountInformation(authToken, serial);
	return acc;
}

AccountInformation AccountInformation::asGuest() {
	return AccountInformation();
}
