#pragma once
#include "ICommand.h"
#include <fstream>
#include "../../../Utils/SkinUtil.h"
#include "../../../resource.h"
#include <thread>
#include "../../Scripting/ScriptManager.h"


class TestCommand : public IMCCommand {
public:
	TestCommand();
	~TestCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
