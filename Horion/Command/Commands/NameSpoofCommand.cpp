#include "NameSpoofCommand.h"

NameSpoofCommand::NameSpoofCommand() : IMCCommand("namespoof", "Spoof your name on realms/bds", "<name/reset> <name>") {
	registerAlias("ns");
	registerAlias("fakename");
}

NameSpoofCommand::~NameSpoofCommand() {
}

std::string string_replace(const std::string& s, const std::string& findS, const std::string& replaceS) {
	std::string result = s;
	auto pos = s.find(findS);
	if (pos == std::string::npos) {
		return result;
	}
	result.replace(pos, findS.length(), replaceS);
	return string_replace(result, findS, replaceS);
}

std::string parse(const std::string& s) {
	static std::vector<std::pair<std::string, std::string> > patterns = {
		{"\\\\", "\\"},
		{"\\n", "\n"},
		{"\\r", "\r"},
		{"\\t", "\t"},
		{"\\\"", "\""}};
	std::string result = s;
	for (const auto& p : patterns) {
		result = string_replace(result, p.first, p.second);
	}
	return result;
}

bool NameSpoofCommand::execute(std::vector<std::string>* args) {
	if (args->at(1) == "name" && args->size() > 2) {
		std::ostringstream os;
		for (int i = 2; i < args->size(); i++) {
			if (i > 2)
				os << " ";
			os << args->at(i);
		}
		TextHolder* name = new TextHolder(parse(os.str()));
		g_Data.setFakeName(name);
		clientMessageF("[%sHorion%s] %sSet fakename to %s%s%s, please reconnect!", GOLD, WHITE, GREEN, GRAY, name->getText(), GREEN);
		return true;
	} else if (args->at(1) == "reset") {
		g_Data.setFakeName(NULL);
		clientMessageF("[%sHorion%s] %sReset fakename!", GOLD, WHITE, GREEN);
		return true;
	}
	return false;
}
