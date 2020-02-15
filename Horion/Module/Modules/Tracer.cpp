#include "Tracer.h"

Tracer::Tracer() : IModule('R', Category::VISUAL, "Draws lines to ESP highlighted entities") {
}

Tracer::~Tracer() {
}

const char* Tracer::getModuleName() {
	return ("Tracer");
}

void Tracer::onPostRender(C_MinecraftUIRenderContext* ctx) {
	g_Data.forEachEntity([](C_Entity* ent, bool valid) {
		if(Target::isValidTarget(ent) && ent != g_Data.getLocalPlayer()) DrawUtils::drawTracer(ent);
	});
}
