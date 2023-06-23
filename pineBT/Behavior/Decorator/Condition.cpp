#include "Condition.h"

#include <assert.h>

using namespace pineBT;

BehaviorSchema Condition::schema = BehaviorSchema::inherit(
	"Condition",
	Decorator::schema,
	BehaviorSchema::Options().build()
		.enumeration("mode", { "instant", "continuous" })
		.boolean("negate")
	.end()
);

Behavior::Result Condition::update()
{
	const bool expected = static_cast<std::underlying_type<Logic>::type>(logic);
	if (check() == expected)
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
