#include "Tracer.h"
#include "../../DrawUtils.h"

Tracer::Tracer() : IModule('R', Category::VISUAL, "Draws lines to ESP highlighted entities") {
}

Tracer::~Tracer() {
}

const char* Tracer::getModuleName() {
	return ("Tracer");
}

void Tracer::onPostRender(C_MinecraftUIRenderContext* ctx) {
	if (!GameData::canUseMoveKeys()) return;
	g_Data.forEachEntity([](C_Entity* ent, bool valid) {
		DrawUtils::setColor(255, 255, 255, 1);
		if(Target::isValidTarget(ent) && ent != g_Data.getLocalPlayer()) DrawUtils::drawTracer(ent);
	});
}
