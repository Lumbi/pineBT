#include "Decorator.h"

#include <assert.h>

using namespace pineBT;

void Decorator::setChild(Behavior* newChild)
{
	this->child = newChild;
}

void Decorator::abort()
{
	assert(child);
	child->abort();
	Behavior::abort();
}
