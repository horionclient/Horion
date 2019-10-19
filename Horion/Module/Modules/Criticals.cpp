#include "Criticals.h"



Criticals::Criticals() : IModule(0x0, COMBAT, "Each hit is a critical hit")
{
}


Criticals::~Criticals()
{
}

const char* Criticals::getModuleName()
{
	return ("Criticals");
}

