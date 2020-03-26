#pragma once
#include "ICommand.h"
#include <fstream>
#include "../../../Utils/SkinUtil.h"
#include "../../../resource.h"
#include <thread>
#include "../../../Utils/ChakraHelper.h"

class TestCommand : public IMCCommand {
public:
	TestCommand();
	~TestCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
