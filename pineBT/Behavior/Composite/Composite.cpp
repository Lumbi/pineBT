#include "Composite.h"

using namespace pineBT;

BehaviorSchema Composite::schema = {
	"Composite",
	BehaviorSchema::Hierarchy::MANY
};

void Composite::addChild(std::unique_ptr<Behavior> child)
{
	children.emplace_back(std::move(child));
}

void Composite::abort()
{
	for (auto&& child : children)
	{
		child->abort();
	}
	Behavior::abort();
}
