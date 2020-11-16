#include "MatrixStack.h"

MatrixStackRef::~MatrixStackRef() {
	this->stack->pop();
}
