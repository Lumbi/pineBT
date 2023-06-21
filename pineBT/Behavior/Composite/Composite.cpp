#include "Composite.h"

using namespace pineBT;

void Composite::addChild(Behavior* child)
{
	children.push_back(child);
}
