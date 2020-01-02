#pragma once
#include "ICommand.h"

#include "../../../resource.h"
#include "../../../include/WinHttpClient.h"
#include <fstream>

class TestCommand :
	public IMCCommand
{
private:
	HGLOBAL hMemoryGeometry;
	HGLOBAL hMemorySteve;
public:
	TestCommand();
	~TestCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};

