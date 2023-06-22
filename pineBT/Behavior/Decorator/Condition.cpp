#include "Condition.h"

#include <assert.h>

using namespace pineBT;

Behavior::Result Condition::update()
{
	if (check())
	{
		switch (mode)
		{
		case Mode::INSTANT: 
			if (child)
				return child->run();
			else
				return Result::SUCCESS;
			break;

		case Mode::CONTINUOUS: 
			if (child && child->run() != Result::SUCCESS) 
				return child->getResult();
			else
				return Result::RUNNING;
			break;
		};
	}
	else if (child)
	{
		child->abort();
	}

	return Result::FAILURE;
}
