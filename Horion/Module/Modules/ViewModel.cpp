#include "ViewModel.h"


ViewModel::ViewModel() : IModule(0, Category::VISUAL, "Custom item view model") {
	registerBoolSetting("Reset", &Reset, Reset);
	registerBoolSetting("Translate", &doTranslate, doTranslate);
	registerBoolSetting("Scale", &doScale, doScale);

	registerFloatSetting("TranslateX", &xTrans, 0.f, -2.f, 2.f);
	registerFloatSetting("TranslateY", &yTrans, 0.f, -2.f, 2.f);
	registerFloatSetting("TranslateZ", &zTrans, 0.f, -2.f, 2.f);

	registerFloatSetting("ScaleX", &xMod, 1.f, 0.f, 2.f);
	registerFloatSetting("ScaleY", &yMod, 1.f, 0.f, 2.f);
	registerFloatSetting("ScaleZ", &zMod, 1.f, 0.f, 2.f);
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

