#include "Tracer.h"



Tracer::Tracer() : IModule('R', Category::VISUAL, "Draws lines to ESP highlighted entities")
{
}


Tracer::~Tracer()
{
}

const char* Tracer::getModuleName()
{
	return ("Tracer");
}