#include "ViewModel.h"


ViewModel::ViewModel() : IModule(0, Category::VISUAL, "Custom item view model") {
	this->registerBoolSetting("Reset", &this->Reset, this->Reset);
	this->registerBoolSetting("Translate", &this->doTranslate, this->doTranslate);
	this->registerBoolSetting("Scale", &this->doScale, this->doScale);

	this->registerFloatSetting("TranslateX", &this->xTrans, 0.f, -2.f, 2.f);
	this->registerFloatSetting("TranslateY", &this->yTrans, 0.f, -2.f, 2.f);
	this->registerFloatSetting("TranslateZ", &this->zTrans, 0.f, -2.f, 2.f);

	this->registerFloatSetting("ScaleX", &this->xMod, 1.f, 0.f, 2.f);
	this->registerFloatSetting("ScaleY", &this->yMod, 1.f, 0.f, 2.f);
	this->registerFloatSetting("ScaleZ", &this->zMod, 1.f, 0.f, 2.f);
}

ViewModel::~ViewModel() {
}

const char* ViewModel::getModuleName() {
	return "ViewModel";
}

void ViewModel::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (Reset) {
		xTrans = 0.f;
		yTrans = 0.f;
		zTrans = 0.f;

		xMod = 1.f;
		yMod = 1.f;
		zMod = 1.f;
		Reset = false;
	}
}

