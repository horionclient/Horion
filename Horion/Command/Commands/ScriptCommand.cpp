#include "ScriptCommand.h"

ScriptCommand::ScriptCommand() : IMCCommand("script", "Manage scripts", "<load|unloadall>") {
}

ScriptCommand::~ScriptCommand() {
}

bool ScriptCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);
	std::string action = args->at(1);
	if (action == "load") {
		HorionDataPacket packet;
		packet.cmd = CMD_FOLDERCHOOSER;
		auto temp = std::shared_ptr<unsigned char[]>(new unsigned char[300]);
		packet.data.swap(temp);
		memset(packet.data.get(), 0, 300);
		strcpy_s((char*)packet.data.get(), 200, "{\"title\": \"Select a Script Folder\", \"filter\":\".js\"}");
		packet.dataArraySize = (int)strlen((char*)packet.data.get());
		packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
			if (pk->params[0] != 1) {  // Dialog Canceled, reset geo
				auto box = g_Data.addInfoBox("Scripting", "Invalid Folder");
				box->closeTimer = 1;
				return;
			}

			wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
			std::wstring jsonDataStr(jsonData);

			json parsed = json::parse(jsonDataStr);
			if (parsed["path"].is_string()) {
				auto box = g_Data.addInfoBox("Importing Script", "Please wait...");
				std::thread gamer([parsed, box]() {
					auto result = scriptMgr.importScriptFolder(parsed["path"].get<std::string>());
					if (result)
						box->fadeTarget = 0;
					else {
						box->message = "Script import error, \ncheck the console";
						box->closeTimer = 2;
					}
				});
				gamer.detach();
			}
		});

		g_Data.sendPacketToInjector(packet);
		return true;
	} else if (action == "unloadall") {
		scriptMgr.unloadAllScripts();
		return true;
	}
	

	return false;
}
