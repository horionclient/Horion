#include "Tracer.h"



Tracer::Tracer() : IModule('R', VISUAL)
{
}


Tracer::~Tracer()
{
}

std::string Tracer::getModuleName()
{
	return std::string("Tracer");
}