#include "Composite.h"

using namespace pineBT;

void Composite::addChild(std::unique_ptr<Behavior> child)
{
	children.push_back(std::move(child));
}
