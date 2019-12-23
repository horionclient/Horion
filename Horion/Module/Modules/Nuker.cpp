#include "Nuker.h"



Nuker::Nuker() : IModule(VK_NUMPAD5, Category::BUILD, "Break multiple blocks at once") {
	this->registerIntSetting(std::string("radius"), &this->nukerRadius, this->nukerRadius, 1, 10);
	this->registerBoolSetting(std::string("veinminer"), &this->veinMiner, this->veinMiner);
}


Nuker::~Nuker() {
}

const char* Nuker::getModuleName() {
	return ("Nuker");
}