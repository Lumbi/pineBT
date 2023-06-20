#include "Condition.h"

#include <assert.h>

using namespace pineBT;

Behavior::Result Condition::update()
{
	assert(child);

	if (!check()) 
	{
		child->abort();
		return Result::FAILURE;
	}
	else
	{
		return child->run();
	}
}
