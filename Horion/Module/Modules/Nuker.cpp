#include "Nuker.h"



Nuker::Nuker() : IModule(VK_NUMPAD5, BUILD)
{
	this->registerIntSetting(std::string("radius"), &this->nukerRadius, this->nukerRadius, 1, 20);
}


Nuker::~Nuker()
{
}

const char* Nuker::getModuleName()
{
	return ("Nuker");
}