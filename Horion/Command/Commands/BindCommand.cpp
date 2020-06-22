#include "BindCommand.h"

#include "../../Module/ModuleManager.h"

BindCommand::BindCommand() : IMCCommand("bind", "Binds modules to specific keys", "<module> <key>") {
	registerAlias("b");
}

BindCommand::~BindCommand() {
}

bool BindCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);
	std::string moduleName = args->at(1);
	assertTrue(moduleName.size() > 0);

	auto modOpt = moduleMgr->getModuleByName(moduleName);
	if (!modOpt.has_value()) {
		clientMessageF("%sCould not find module with name: %s", RED, moduleName.c_str());
		return true;
	}
	auto mod = modOpt.value();

	if (args->size() >= 3) {
		std::string key = args->at(2);

		assertTrue(key.size() > 0);

		if (key.size() > 1) {
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			const char* needle = key.c_str();

			if (key == "none") {
				mod->setKeybind(0x0);
				clientMessageF("%sSuccessfully unbound %s!", GREEN, mod->getModuleName());
				return true;
			}

			for (int i = 0; i < 190; i++) {
				const char* haystack = KeyNames[i];
				size_t len = strlen(needle) + 1;
				char* haystackLowercase = new char[len];
				for (int i = 0; i < len; i++)
					haystackLowercase[i] = tolower(haystack[i]);

				if (strcmp(needle, haystackLowercase) == 0) {
					mod->setKeybind(i);
					clientMessageF("%sThe keybind of %s is now '%s'", GREEN, mod->getModuleName(), haystack);
					delete[] haystackLowercase;
					return true;
				}
				delete[] haystackLowercase;
			}
			clientMessageF("%sInvalid key!", RED);
			return true;
		}

		int keyCode = (int)key.at(0);
		if (keyCode >= 0x61 && keyCode <= 0x7A)  // Convert key to lower case
			keyCode -= 0x20;

		if (keyCode >= 0x30 && keyCode <= 0x5A) {
			auto modOpt = moduleMgr->getModuleByName(moduleName);
			if (!modOpt.has_value()) {
				clientMessageF("%sCould not find module with name: %s", RED, moduleName.c_str());
			} else {
				modOpt.value()->setKeybind(keyCode);
				clientMessageF("%sThe Keybind of %s is now '%c'", GREEN, mod->getModuleName(), keyCode);
			}
		} else {
			clientMessageF("%sInvalid Key! Outside of ascii range: %X", RED, keyCode);
		}
		return true;
	} else {
		clientMessageF("%s%s is currently bound to %s", RED, mod->getModuleName(), Utils::getKeybindName(mod->getKeybind()));
		return true;
	}
	
}
