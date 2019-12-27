#include "TestCommand.h"


TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "")
{
}


TestCommand::~TestCommand()
{
	if (hMemoryGeometry)
		FreeResource(hMemoryGeometry);
	if (hMemorySteve)
		FreeResource(hMemorySteve);
}

bool TestCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() > 0);
	std::string skinName = args->at(0);
	assertTrue(skinName.size() > 0);

	{
		wchar_t fullUrl[200];
		TextHolder* uuid = &g_Data.getLocalPlayer()->uuid;
		assertTrue(uuid->getTextLength() > 0);
		{
			swprintf_s(fullUrl, 200, L"http://207.180.225.107:5758/api/cosmetic/infoList?api_key=C3DFBFT3r5RXWLrP");
			WinHttpClient client(fullUrl);

			client.SendHttpRequest();

			std::wstring httpResponseCode = client.GetResponseStatusCode();
			if (httpResponseCode != std::to_wstring(200)) {
				this->clientMessageF("Got invalid status code: %S", httpResponseCode.c_str());
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
						logF("Found witchhat!");
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
			swprintf_s(fullUrl, 200, L"http://207.180.225.107:5758/api/user/create/%S?api_key=C3DFBFT3r5RXWLrP", uuid->getText());
			WinHttpClient client(fullUrl);

			client.SendHttpRequest();

			std::wstring httpResponseCode = client.GetResponseStatusCode();
			if (httpResponseCode != std::to_wstring(200)) {
				this->clientMessageF("Got invalid status code (create): %S", httpResponseCode.c_str());
				return true;
			}
		}
		{
			auto currentSkin = g_Data.getLocalPlayer()->getSerializedSkin();
			auto skinData = Utils::base64_encode(reinterpret_cast<unsigned char*>(currentSkin->skinImage.imageData), currentSkin->skinImage.imageDataSize);
			
			swprintf_s(fullUrl, 200, L"http://207.180.225.107:5758/api/cosmetic/buildskin?api_key=C3DFBFT3r5RXWLrP&user=%S&skin_type=%S&skin_data=%S", uuid->getText(), "alex", skinData.c_str());
			WinHttpClient client(fullUrl);

			client.SendHttpRequest();

			std::wstring httpResponseCode = client.GetResponseStatusCode();
			if (httpResponseCode != std::to_wstring(200)) {
				this->clientMessageF("Got invalid status code (create): %S", httpResponseCode.c_str());
				return true;
			}
		}


		return true;
	}

	{
		auto hResourceGeometry = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
		hMemoryGeometry = LoadResource(g_Data.getDllModule(), hResourceGeometry);

		auto sizeGeometry = SizeofResource(g_Data.getDllModule(), hResourceGeometry);
		auto ptrGeometry = LockResource(hMemoryGeometry);

		auto hResourceSteve = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_STEVE), (char*)RT_RCDATA);
		hMemorySteve = LoadResource(g_Data.getDllModule(), hResourceSteve);

		auto sizeSteve = SizeofResource(g_Data.getDllModule(), hResourceSteve);
		auto ptrSteve = LockResource(hMemorySteve);

		std::shared_ptr<TextHolder> newGeometryData = std::make_shared<TextHolder>(ptrGeometry, sizeGeometry, true);
		C_SkinData newSkinData;
		newSkinData.SkinWidth = 128;
		newSkinData.SkinHeight = 128;
		newSkinData.skinData = ptrSteve;
		newSkinData.skinSize = sizeSteve;

		std::shared_ptr<TextHolder> newSkinResourcePatch = std::make_shared<TextHolder>(Utils::base64_decode("ewogICAiZ2VvbWV0cnkiIDogewogICAgICAiYW5pbWF0ZWRfZmFjZSIgOiAiZ2VvbWV0cnkuYW5pbWF0ZWRfZmFjZV9wZXJzb25hXzRjZGJiZmFjYTI0YTk2OGVfMF8wIiwKICAgICAgImRlZmF1bHQiIDogImdlb21ldHJ5LnBlcnNvbmFfNGNkYmJmYWNhMjRhOTY4ZV8wXzAiCiAgIH0KfQo="));

		std::shared_ptr<MinecraftSkinData> skinData = std::make_shared<MinecraftSkinData>(newSkinData, newGeometryData, newSkinResourcePatch);
		g_Data.setOverwrittenSkin(skinData);
	}

	return true;
}