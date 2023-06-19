#include "Composite.h"

using namespace matsuBT;

void Composite::addChild(std::unique_ptr<Behavior> child)
{
	children.push_back(std::move(child));
}
