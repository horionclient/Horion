#include "Tracer.h"



Tracer::Tracer() : IModule('R', VISUAL)
{
}


Tracer::~Tracer()
{
}

const char* Tracer::getModuleName()
{
	return ("Tracer");
}