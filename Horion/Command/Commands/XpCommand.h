#pragma once

#include "ICommand.h"

class XpCommand : public IMCCommand {
public:
	XpCommand();
	~XpCommand();

	bool execute(std::vector<std::string>* args) override;
};
