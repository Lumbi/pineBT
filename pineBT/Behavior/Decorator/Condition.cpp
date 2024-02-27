#include "Condition.h"

#include <assert.h>

using namespace pineBT;

static const char* OPT_MODE = "mode";
static const int OPT_MODE_COUNT = 2;
static const char* OPT_NEGATE = "negate";

BehaviorSchema Condition::schema = BehaviorSchema::inherit(
	"Condition",
	Decorator::schema,
	BehaviorSchema::Options().build()
		.enumeration(OPT_MODE, OPT_MODE_COUNT)
		.boolean(OPT_NEGATE)
	.end()
);

void Condition::configure(const Option& option)
{
	if (option.key == OPT_MODE)
	{
		assert(option.value.tag == Option::Value::Type::CASE);
		setMode(static_cast<Mode>(option.value.asCase));
	}
	else if (option.key == OPT_NEGATE)
	{
		assert(option.value.tag == Option::Value::Type::BOOLEAN);
		if (option.value.asBoolean)
			setLogic(Logic::NEGATE);
		else
			setLogic(Logic::DEFAULT);
	}
}

void Condition::setMode(Mode newMode)
{
	this->mode = newMode;
}

void Condition::setLogic(Logic newLogic)
{
	this->logic = newLogic;
}

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
