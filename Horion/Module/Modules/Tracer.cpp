#include "Tracer.h"
#include "../../DrawUtils.h"

#include "../../../SDK/CCamera.h"

Tracer::Tracer() : IModule('R', Category::VISUAL, "Draws lines to ESP highlighted entities") {
}

Tracer::~Tracer() {
}

const char* Tracer::getModuleName() {
	return "Tracer";
}
void Tracer::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	
	g_Data.forEachEntity([&](C_Entity* ent, bool valid) {
		if (ent == player)
			return;
		if (ent->getNameTag()->getTextLength() <= 1 && ent->getEntityTypeId() == 63)
			return;
		if (ent->isInvisible())
			return;
		if (!player->canAttack(ent, false))
			return;
		if (!Target::isValidTarget(ent))
			return;

		DrawUtils::setColor(255, 255, 255, 1);
		DrawUtils::drawTracer(ent);
	});

}
