#include "DebugTask.h"

using namespace pineBT;

Behavior::Result DebugTask::update()
{
	printf("%s\n", message);
	return forceResult;
}
