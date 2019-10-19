#include "Tracer.h"



Tracer::Tracer() : IModule('R', VISUAL, "Draws lines to ESP highlighted entities")
{
}


Tracer::~Tracer()
{
}

const char* Tracer::getModuleName()
{
	return ("Tracer");
}