#include "ExecuteCommand.h"

ExecuteCommand::ExecuteCommand() : IMCCommand("execute", "Executes javascript code", "<code>") {
	registerAlias("exec");
}

ExecuteCommand::~ExecuteCommand() {
}

bool ExecuteCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);
	std::ostringstream os;
	for (int i = 1; i < args->size(); i++) {
		if (i > 1)
			os << " ";
		os << args->at(i);
	}
	auto str = os.str();

	auto wstr = Utils::stringToWstring(str);
	clientMessageF("%sScript returned: %s%S", GOLD, WHITE, scriptMgr.runScript(wstr).c_str());
	return true;
}
