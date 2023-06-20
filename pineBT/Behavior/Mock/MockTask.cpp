#include "MockTask.h"

using namespace pineBT;

Behavior::Result MockTask::update()
{
	printf("%s\n", message);
	return forceResult;
}

void MockTask::setForceResult(Result forceResult)
{
	this->forceResult = forceResult;
}
