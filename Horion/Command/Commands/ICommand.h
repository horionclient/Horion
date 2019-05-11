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
	std::string strCommand;
protected:
	inline void assertTrue(bool condition) { if (!condition) { throw std::exception(); } };
	inline float assertFloat(std::string string) {
		return (float)std::stof(string);
	};
public:
	ICommand(const char* command, char* description, char* usage);
	~ICommand();

	virtual const char* getCommand() { return _command; };
	virtual std::string getCommandStr() { return strCommand; };
	virtual const char* getDescription() { return _description; };
	virtual const char* getUsage() { return _usage; };
	virtual bool execute(std::vector<std::string>* args) = 0;
};