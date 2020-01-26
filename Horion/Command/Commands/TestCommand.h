#pragma once
#include "ICommand.h"

//#define HTTP_TEST

#ifdef HTTP_TEST
#include "../../../include/WinHttpClient.h"
#endif

class TestCommand : public IMCCommand {
public:
	TestCommand();
	~TestCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
