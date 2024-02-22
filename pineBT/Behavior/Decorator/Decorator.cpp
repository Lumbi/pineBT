#include "Decorator.h"

#include <assert.h>

using namespace pineBT;

BehaviorSchema Decorator::schema = {
	"Decorator",
	BehaviorSchema::Hierarchy::ONE
};

void Decorator::addChild(std::unique_ptr<Behavior> newChild)
{
	child.swap(newChild);
}

void Decorator::setChild(std::unique_ptr<Behavior> newChild)
{
	addChild(std::move(newChild));
}

Behavior* Decorator::getChild() const
{
	return child.get();
}

void Decorator::abort()
{
	if (child) child->abort();	
	Behavior::abort();
}
