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
void Tracer::onLevelRender() {

	auto cameraMgr = g_Data.getClientInstance()->getCameraManager();
	if(cameraMgr == nullptr)
		return;
	auto cam = cameraMgr->getCameraOrDebugCamera();
	if(cam == nullptr)
		return;
	vec3_t forward{};
	cam->getForwardVector(&forward);

	const vec3_t origin = g_Data.getClientInstance()->levelRenderer->origin.add(forward.mul(0.2f) /*place the start of the line slightly forward so it won't get clipped*/);
	g_Data.forEachEntity([&](C_Entity* ent, bool valid) {
	  if(ent != g_Data.getLocalPlayer() && Target::isValidTarget(ent)){
		  DrawUtils::setColor(255, 255, 255, 1);
		  DrawUtils::drawLine3d(origin, *ent->getPos());
	  }
	});
}
