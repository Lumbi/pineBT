#include "Composite.h"

using namespace pineBT;

BehaviorSchema Composite::schema = {
	"Composite",
	BehaviorSchema::Hierarchy::MANY
};

void Composite::addChild(Behavior* child)
{
	children.push_back(child);
}

void Composite::abort()
{
	for (auto&& child : children)
	{
		child->abort();
	}
	Behavior::abort();
}
