#include "Behavior.h"

using namespace pineBT;

Behavior::Result Behavior::run()
{
	if (result != Result::RUNNING) onEnter();
	result = update();
	if (result != Result::RUNNING) onExit(result);
	return result;
}
