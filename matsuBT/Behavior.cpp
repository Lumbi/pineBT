#include "Behavior.h"

using namespace matsuBT;

Behavior::Result Behavior::run()
{
	if (result != Result::RUNNING) onEnter();
	result = update();
	if (result != Result::RUNNING) onExit(result);
	return result;
}
