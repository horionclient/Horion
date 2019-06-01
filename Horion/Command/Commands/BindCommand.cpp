#include "BindCommand.h"



BindCommand::BindCommand() : ICommand("bind", "Binds Modules to specific keys", "<module> <key>")
{
	registerAlias("b");
	num["NUMPAD0"] = 0x60;
	num["NUMPAD1"] = 0x61;
	num["NUMPAD2"] = 0x62;
	num["NUMPAD3"] = 0x63;
	num["NUMPAD4"] = 0x64;
	num["NUMPAD5"] = 0x65;
	num["NUMPAD6"] = 0x66;
	num["NUMPAD7"] = 0x67;
	num["NUMPAD8"] = 0x68;
	num["NUMPAD9"] = 0x69;
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
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
		for (std::map<std::string,int>::iterator it = num.begin(); it != num.end(); ++it)
		{
			std::size_t found = it->first.find(key);
			if (found != std::string::npos)
			{
				IModule* mod = moduleMgr->getModuleByName(moduleName);
				if (mod == nullptr) {
					clientMessageF("%sCould not find Module with name: %s", RED, moduleName.c_str());
				}
				else {
					mod->setKeybind( it->second);
					clientMessageF("%sThe Keybind of %s is now '%s'", GREEN, mod->getModuleName(),it->first.c_str());
				}
				return true;
			}
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