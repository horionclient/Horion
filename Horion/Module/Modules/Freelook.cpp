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
	if(!g_Data.isInGame() || this->resetViewTick > -1){
		this->setEnabled(false);
		return;
	}

	this->redirectMouse = true;
	this->resetViewTick = -1;
	this->initialViewAngles = g_Data.getLocalPlayer()->viewAngles;
	this->lastCameraAngle = {0, 0};
}
void Freelook::onDisable() {
	if(g_Data.isInGame() && this->resetViewTick == -1 && this->redirectMouse){

		this->resetViewTick = this->isThirdPerson ? 0 : 100; // give it time
		if(this->cameraFacesFront){
			lastCameraAngle.y += 180;
			lastCameraAngle = lastCameraAngle.normAngles();
			mouseEnd = this->lastCameraAngle;

			initialViewAngles.y += 180;
			initialViewAngles = initialViewAngles.normAngles();
		}else
			mouseEnd = this->lastCameraAngle;
	}else if(!g_Data.isInGame()){
		this->resetViewTick = -1;
		this->redirectMouse = false;
	}
}
void Freelook::onPostRender(C_MinecraftUIRenderContext* mode) {
	if(!this->redirectMouse)
		return;
	if(!g_Data.isInGame()){
		this->setEnabled(false);
		return;
	}

	if(this->resetViewTick > 0){
		this->resetViewTick--;

		if(this->cameraFacesFront){
			this->lastCameraAngle.x *= -1;
		}
		vec2_t appl = this->initialViewAngles;
		appl = appl.sub(this->lastCameraAngle).normAngles();
		appl.x = this->initialViewAngles.x - this->lastCameraAngle.x; // dont norm this angle
		if(appl.magnitude() < 0.1f)
			this->resetViewTick = 0;

		appl = appl.div(fmaxf(1, appl.magnitude() / 15));

		this->lastCameraAngle = this->lastCameraAngle.add(appl);
		if(this->cameraFacesFront){
			this->lastCameraAngle.x *= -1;
		}
	}
}
void Freelook::onTick(C_GameMode* mode) {
	if(this->resetViewTick == 0){
		this->redirectMouse = false;
		this->resetViewTick = -1;
		auto loc = g_Data.getLocalPlayer();
		vec2_t l = initialViewAngles;
		l = l.sub(mouseEnd).normAngles();
		l.x = this->initialViewAngles.x - mouseEnd.x; // dont norm this angle

		l.x *= -1;
		loc->applyTurnDelta(&l);

		this->lastCameraAngle = {0, 0};
	}
}
