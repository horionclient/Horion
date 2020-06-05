#include "CoordsCommand.h"

CoordsCommand::CoordsCommand() : IMCCommand("coords", "Prints your coordinates", "") {
	registerAlias("pos");
}

CoordsCommand::~CoordsCommand() {
}

bool CoordsCommand::execute(std::vector<std::string>* args) {
	vec3_t* pos = g_Data.getLocalPlayer()->getPos();
	float yPos = pos->y - 1.62f;
	clientMessageF("[%sHorion%s] %sX: %.2f Y: %.2f Z: %.2f", GOLD, WHITE, GREEN, pos->x, yPos /* eye height */, pos->z);
	return true;
}
