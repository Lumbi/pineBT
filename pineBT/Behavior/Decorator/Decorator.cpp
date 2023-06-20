#include "Decorator.h"

using namespace pineBT;

void Decorator::setChild(std::unique_ptr<Behavior> newChild)
{
	this->child = std::move(newChild);
}
