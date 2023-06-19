#include "DebugTask.h"

using namespace matsuBT;

Behavior::Result DebugTask::update()
{
	printf("%s\n", message);
	return forceResult;
}
