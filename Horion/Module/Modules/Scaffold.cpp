#include "Scaffold.h"



Scaffold::Scaffold() : IModule(VK_NUMPAD1)
{
}


Scaffold::~Scaffold()
{
}

std::string Scaffold::getModuleName()
{
	return std::string("Scaffold");
}

void Scaffold::onPostRender()
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
	blockBelow.y -= g_Data.getLocalPlayer()->height + 0.3f;
	blockBelow = blockBelow.floor();
	DrawUtils::drawBox(blockBelow, vec3_t(blockBelow).add(1), 1);
}
