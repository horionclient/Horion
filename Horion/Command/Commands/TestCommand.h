#pragma once
#include "ICommand.h"
#include <fstream>
#include "../../../Utils/SkinUtil.h"
#include "../../../resource.h"
#include <thread>

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
