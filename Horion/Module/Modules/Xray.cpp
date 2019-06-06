#include "Xray.h"



Xray::Xray() : IModule('A', VISUAL)
{
}


Xray::~Xray()
{
}

const char* Xray::getModuleName()
{
	return ("X-Ray");
}