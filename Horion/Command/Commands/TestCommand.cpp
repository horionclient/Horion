#include "TestCommand.h"

TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "") {
}

TestCommand::~TestCommand() {
	if (hMemoryGeometry)
		FreeResource(hMemoryGeometry);
	if (hMemorySteve)
		FreeResource(hMemorySteve);
}

bool TestCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 0);
	std::string skinName = args->at(0);
	assertTrue(skinName.size() > 0);

	{
		const char* apiKey = "C3DFBFT3r5RXWLrP";
		auto currentSkin = g_Data.getLocalPlayer()->getSerializedSkin();
		TextHolder* uuid = g_Data.getLocalPlayer()->getUUID();

		wchar_t fullUrl[200];

		assertTrue(uuid->getTextLength() > 0);

		std::unique_ptr<unsigned int[]> imageDataResized(new unsigned int[64 * 64]);

		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				if (y > currentSkin->skinImage.height || x > currentSkin->skinImage.width)
					continue;
				imageDataResized[x + y * 64] = currentSkin->skinImage.getDataAt(x, y);
			}
		}

		
		/*{
			std::ofstream moin("C:\\Users\\gcaker\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\skin.dat", std::ios::out | std::ios::binary);
			moin.write(reinterpret_cast<char*>(imageDataResized.get()), 64 * 64 * 4);
			moin.close();
		}*/
		{
			GamerTextHolder boi;
			currentSkin->geometry.getAsString(boi);
			std::ofstream moin("C:\\Users\\gcaker\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\orig.dat", std::ios::out | std::ios::binary);
			moin.write(reinterpret_cast<char*>(boi.getText()), boi.getTextLength());
			moin.close();
			boi.getText();
			
		}

		logF("skin: %llX", currentSkin);

		{
			swprintf_s(fullUrl, L"http://207.180.225.107:5758/api/cosmetic/infoList?api_key=%S", apiKey);
			WinHttpClient client(fullUrl);

			client.SendHttpRequest();

			std::wstring httpResponseCode = client.GetResponseStatusCode();
			if (httpResponseCode != std::to_wstring(200)) {
				logF("Got invalid status code: %S", httpResponseCode.c_str());
				return true;
			}
			std::wstring content = client.GetResponseContent();

			using json = nlohmann::json;
			auto obj = json::parse(content);
			bool itemAvailable = false;
			for (auto it = obj.begin(); it != obj.end(); it++) {
				auto val = *it;
				if (val.is_object()) {
					auto name = val.at("name");
					if (name.is_string() && name.get<std::string>() == "Witchhat") {
						itemAvailable = true;
						break;
					}
				}
			}

			if (!itemAvailable) {
				this->clientMessageF("Could not find item!");
				return true;
			}
		}

		{
			swprintf_s(fullUrl, L"http://207.180.225.107:5758/api/user/set/style/%S?api_key=%S&head=%S&body=%S&custom=%S", uuid->getText(), apiKey, "Witchhat", "Jetpack", "Shield");
			WinHttpClient client(fullUrl);

			client.SendHttpRequest();

			std::wstring httpResponseCode = client.GetResponseStatusCode();
			if (httpResponseCode != std::to_wstring(200)) {
				logF("Got invalid status code (setstyle): %S leng: %i", httpResponseCode.c_str(), httpResponseCode.size());
				return true;
			}
		}

		{
			swprintf_s(fullUrl, L"http://207.180.225.107:5758/api/user/create/%S?api_key=%S", uuid->getText(), apiKey);
			WinHttpClient client(fullUrl);

			client.SendHttpRequest();

			std::wstring httpResponseCode = client.GetResponseStatusCode();
			if (httpResponseCode != std::to_wstring(200)) {
				logF("Got invalid status code (create): %S leng: %i", httpResponseCode.c_str(), httpResponseCode.size());
				return true;
			}
		}
		{
			swprintf_s(fullUrl, 200, L"http://207.180.225.107:5758/api/cosmetic/buildskin");

			auto skinData = Utils::url_encode(Utils::base64_encode(reinterpret_cast<unsigned char*>(imageDataResized.get()), 64 * 64 * 4));

			std::shared_ptr<char[]> postData(new char[200 + skinData.size()]);
			int postDataSize = sprintf_s(postData.get(), 200 + skinData.size(), "api_key=%s&user=%s&skin_type=%s&skin_data=%s", apiKey, uuid->getText(), "alex", skinData.c_str());

			WinHttpClient client(fullUrl);

			client.SetPostDataToSend(postData, postDataSize);
			client.SetAdditionalRequestHeaders(L"Content-Type: application/x-www-form-urlencoded");
			client.SendHttpRequest(L"POST");

			std::wstring httpResponseCode = client.GetResponseStatusCode();
			if (httpResponseCode != std::to_wstring(200)) {
				logF("Got invalid status code (build): %S leng: %i", httpResponseCode.c_str(), httpResponseCode.size());
				return true;
			} else {
				auto resp = client.GetResponseContent();
				using json = nlohmann::json;
				auto obj = json::parse(resp);

				if (obj["error"].is_string()) {
					logF("Skin has error: %s", obj.at("error").get<std::string>().c_str());
				} else {
					std::string skinData = Utils::base64_decode(obj.at("skin_data").get<std::string>());
					std::string geometryData = obj.at("geometry").get<std::string>();
					std::string geometryName = obj.at("geometry_name").get<std::string>();

					C_MinecraftImage newSkin;
					newSkin.height = 128;
					newSkin.width = 128;
					char* allocSkin = new char[skinData.size()];
					memcpy(allocSkin, skinData.c_str(), skinData.size());
					newSkin.imageDataSize = skinData.size();
					newSkin.imageData = allocSkin;

					auto patch = json();
					patch["geometry"]["default"] = geometryName;

					std::shared_ptr<TextHolder> newSkinResourcePatch = std::make_shared<TextHolder>(patch.dump());

					std::shared_ptr<MinecraftSkinData> data = std::make_shared<MinecraftSkinData>(newSkin, std::make_shared<TextHolder>(geometryData), newSkinResourcePatch);
					g_Data.setOverwrittenSkin(data);
					this->clientMessageF("Rejoin the server to apply the skin");
				}
			}
		}

		return true;
	}

	/*{
		auto hResourceGeometry = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
		hMemoryGeometry = LoadResource(g_Data.getDllModule(), hResourceGeometry);

		auto sizeGeometry = SizeofResource(g_Data.getDllModule(), hResourceGeometry);
		auto ptrGeometry = LockResource(hMemoryGeometry);

		auto hResourceSteve = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_STEVE), (char*)RT_RCDATA);
		hMemorySteve = LoadResource(g_Data.getDllModule(), hResourceSteve);

		auto sizeSteve = SizeofResource(g_Data.getDllModule(), hResourceSteve);
		auto ptrSteve = LockResource(hMemorySteve);

		std::shared_ptr<TextHolder> newGeometryData = std::make_shared<TextHolder>(ptrGeometry, sizeGeometry, true);
		C_MinecraftImage newSkinData;
		newSkinData.width = 128;
		newSkinData.height = 128;
		newSkinData.imageData = ptrSteve;
		newSkinData.imageDataSize = sizeSteve;

		std::shared_ptr<TextHolder> newSkinResourcePatch = std::make_shared<TextHolder>(Utils::base64_decode("ewogICAiZ2VvbWV0cnkiIDogewogICAgICAiYW5pbWF0ZWRfZmFjZSIgOiAiZ2VvbWV0cnkuYW5pbWF0ZWRfZmFjZV9wZXJzb25hXzRjZGJiZmFjYTI0YTk2OGVfMF8wIiwKICAgICAgImRlZmF1bHQiIDogImdlb21ldHJ5LnBlcnNvbmFfNGNkYmJmYWNhMjRhOTY4ZV8wXzAiCiAgIH0KfQo="));

		std::shared_ptr<MinecraftSkinData> skinData = std::make_shared<MinecraftSkinData>(newSkinData, newGeometryData, newSkinResourcePatch);
		g_Data.setOverwrittenSkin(skinData);
	}

	return true;*/
}
