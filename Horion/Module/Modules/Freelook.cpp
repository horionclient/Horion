#include "Freelook.h"

#include "../../../Utils/Logger.h"

Freelook::Freelook(): IModule(0, Category::VISUAL, "Move straight while moving your camera around") {
}
Freelook::~Freelook() {
}
const char* Freelook::getModuleName() {
	return "Freelook";
}
vec2_t mouseEnd = {0, 0};
void Freelook::onEnable() {
	if(!g_Data.isInGame() || resetViewTick > -1){
		setEnabled(false);
		return;
	}

	redirectMouse = true;
	resetViewTick = -1;
	initialViewAngles = g_Data.getLocalPlayer()->viewAngles;
	lastCameraAngle = {0, 0};
}
void Freelook::onDisable() {
	if(g_Data.isInGame() && resetViewTick == -1 && redirectMouse){

		resetViewTick = isThirdPerson ? 0 : 100; // give it time
		if(cameraFacesFront){
			lastCameraAngle.y += 180;
			lastCameraAngle = lastCameraAngle.normAngles();
			mouseEnd = lastCameraAngle;

			initialViewAngles.y += 180;
			initialViewAngles = initialViewAngles.normAngles();
		} else {
			mouseEnd = lastCameraAngle;
			mouseEnd.y += 180;
		}
	}else if(!g_Data.isInGame()){
		resetViewTick = -1;
		redirectMouse = false;
	}
}
void Freelook::onPostRender(C_MinecraftUIRenderContext* mode) {
	if(!redirectMouse)
		return;
	if(!g_Data.isInGame()){
		setEnabled(false);
		return;
	}

	if(resetViewTick > 0){
		resetViewTick--;

		if(cameraFacesFront){
			lastCameraAngle.x *= -1;
		}
		vec2_t appl = initialViewAngles;
		appl = appl.sub(lastCameraAngle).normAngles();
		appl.x = initialViewAngles.x - lastCameraAngle.x; // dont norm this angle
		if(appl.magnitude() < 0.1f)
			resetViewTick = 0;

		appl = appl.div(fmaxf(1, appl.magnitude() / 15));

		lastCameraAngle = lastCameraAngle.add(appl);
		if(cameraFacesFront){
			lastCameraAngle.x *= -1;
		}
	}
}
void Freelook::onTick(C_GameMode* mode) {
	if(resetViewTick == 0){
		redirectMouse = false;
		resetViewTick = -1;
		auto loc = g_Data.getLocalPlayer();
		vec2_t l = initialViewAngles;
		l = l.sub(mouseEnd).normAngles();
		l.x = this->initialViewAngles.x - mouseEnd.x; // dont norm this angle

		l.x *= -1;
		loc->applyTurnDelta(&l);

		lastCameraAngle = {0, 0};
	}
}
