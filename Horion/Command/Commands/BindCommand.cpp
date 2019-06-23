#include "BindCommand.h"



BindCommand::BindCommand() : ICommand("bind", "Binds Modules to specific keys", "<module> <key>")
{
	registerAlias("b");
}


BindCommand::~BindCommand()
{
}

bool BindCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() >= 3);
	std::string moduleName = args->at(1); 
	std::string key = args->at(2);

	assertTrue(moduleName.size() > 0);
	assertTrue(key.size() > 0);

	if (key.size() > 1) {
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		const char* needle = key.c_str();

		for(int i = 0; i < 190; i++)
		{
			const char* haystack = KeyNames[i];
			size_t len = strlen(needle) + 1;
			char* haystackLowercase = new char[len];
			for (int i = 0; i < len; i++)
				haystackLowercase[i] = tolower(haystack[i]);

			//clientMessageF("%s", needleLowercase);

			if(strcmp(needle, haystackLowercase) == 0)
			{
				IModule* mod = moduleMgr->getModuleByName(moduleName);
				if (mod == nullptr) {
					clientMessageF("%sCould not find Module with name: %s", RED, moduleName.c_str());
				}
				else {
					mod->setKeybind(i);
					clientMessageF("%sThe Keybind of %s is now '%s'", GREEN, mod->getModuleName(), haystack);
				}
				delete[] haystackLowercase;
				return true;
			}
			delete[] haystackLowercase;
		}
		clientMessageF("%sInvalid Key!", RED);
		return true;
	}

	int keyCode = (int) key.at(0);
	if (keyCode >= 0x61 && keyCode <= 0x7A) // Convert key to lower case
		keyCode -= 0x20;

	if (keyCode >= 0x30 && keyCode <= 0x5A) {
		IModule* mod = moduleMgr->getModuleByName(moduleName);
		if (mod == nullptr) {
			clientMessageF("%sCould not find Module with name: %s", RED, moduleName.c_str());
		}
		else {
			mod->setKeybind(keyCode);
			clientMessageF("%sThe Keybind of %s is now '%c'", GREEN, mod->getModuleName(), keyCode);
		}
	}
	else {
		clientMessageF("%sInvalid Key! Outside of ascii range: %X", RED, keyCode);
	}
	return true;
}