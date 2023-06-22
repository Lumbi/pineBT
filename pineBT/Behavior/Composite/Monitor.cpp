#include "Monitor.h"

using namespace pineBT;

void Monitor::addCondition(Behavior* condition)
{
	children.insert(children.begin(), condition);
}

void Monitor::addAction(Behavior* action)
{
	children.push_back(action);
}
