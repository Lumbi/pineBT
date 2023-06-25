#include "Decorator.h"

#include <assert.h>

using namespace pineBT;

BehaviorSchema Decorator::schema = {
	"Decorator",
	BehaviorSchema::Hierarchy::ONE
};

void Decorator::addChild(Behavior* newChild)
{
	this->child = newChild;
}

void Decorator::setChild(Behavior* newChild)
{
	addChild(newChild);
}

Behavior* Decorator::getChild() const
{
	return child;
}

void Decorator::abort()
{
	if (child) child->abort();	
	Behavior::abort();
}
