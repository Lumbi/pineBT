#include "Monitor.h"

using namespace pineBT;

void Monitor::addCondition(std::unique_ptr<Behavior> condition)
{
	children.emplace(children.begin(), std::move(condition));
}

void Monitor::addAction(std::unique_ptr<Behavior> action)
{
	children.emplace_back(std::move(action));
}
