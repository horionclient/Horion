#include "Tracer.h"



Tracer::Tracer() : IModule('R')
{
}


Tracer::~Tracer()
{
}

std::string Tracer::getModuleName()
{
	return std::string("Tracer");
}