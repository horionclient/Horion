#pragma once

#include <vector>
#include <string>
#include "../../Module/ModuleManager.h"
#include "../../../Memory/GameData.h"

#ifndef ASSERT_T
#define ASSERT_T(x) if(!(x)) return false;
#endif

class ICommand {
private:
	const char* _command;
	const char* _description;
	const char* _usage;
	std::vector<std::string> aliasList;
protected:
	inline void assertTrue(bool condition) { if (!condition) { throw std::exception(); } };
	inline float assertFloat(std::string string) { return std::stof(string); };

	inline void registerAlias(const char* str) { 
		std::string ss = str;
		std::transform(ss.begin(), ss.end(), ss.begin(), ::tolower);
		aliasList.push_back(ss);
	};

	inline void clientMessageF(const char * fmt, ...) {
		va_list arg;
		va_start(arg, fmt);
		g_Data.getGuiData()->displayClientMessageVA(fmt, arg);
		va_end(arg);
	};
public:
	ICommand(const char* command, char* description, char* usage);
	~ICommand();

	virtual const char* getCommand() { return _command; };
	virtual std::vector<std::string>* getAliasList() { return &aliasList; };
	virtual const char* getDescription() { return _description; };
	virtual const char* getUsage() { return _usage; };
	virtual bool execute(std::vector<std::string>* args) = 0;
};