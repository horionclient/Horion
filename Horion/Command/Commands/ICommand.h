#pragma once

#include "../../../Memory/GameData.h"
#include "../../FriendList/FriendList.h"

#ifndef ASSERT_T
#define ASSERT_T(x) \
	if (!(x)) return false;
#endif

class IMCCommand {
private:
	const char* _command;
	const char* _description;
	const char* _usage;
	std::vector<std::string> aliasList;

protected:
	inline void assertTrue(bool condition) {
		if (!condition) {
			throw std::exception();
		}
	};
	inline float assertFloat(std::string string) { return std::stof(string); };
	inline int assertInt(std::string string) { return std::stoi(string); };

	void registerAlias(const char* str);

	void clientMessageF(const char* fmt, ...);

public:
	IMCCommand(const char* command, const char* description, const char* usage);
	virtual ~IMCCommand();

	virtual const char* getCommand() { return _command; };
	virtual std::vector<std::string>* getAliasList() { return &aliasList; };
	virtual const char* getDescription() { return _description; };
	virtual const char* getUsage(const char* alias) { return _usage; };
	virtual bool execute(std::vector<std::string>* args) = 0;
};
