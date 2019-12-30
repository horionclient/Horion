#pragma once
#include "ICommand.h"
class CoordsCommand : public IMCCommand {
public:
	CoordsCommand();
	~CoordsCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
