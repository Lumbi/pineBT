#include "BlackboardCondition.h"

#include <assert.h>

using namespace pineBT;

static const char* OPT_KEY = "key";
static const char* OPT_PREDICATE = "predicate";
static const char* OPT_TARGET = "target";

BehaviorSchema BlackboardCondition::schema = BehaviorSchema::inherit(
	"BlackboardCondition",
	Condition::schema,
	BehaviorSchema::Options().build()
		.number(OPT_KEY)
		.enumeration(OPT_PREDICATE, 4)
		.number(OPT_TARGET)
	.end()
);

void BlackboardCondition::configure(const Option& option)
{
	Condition::configure(option);

	if (option.key == OPT_KEY)
	{
		assert(option.value.tag == Option::Value::Type::NUMBER);
		key = static_cast<Blackboard::Key>(option.value.asNumber);
	}
	else if (option.key == OPT_PREDICATE)
	{
		assert(option.value.tag == Option::Value::Type::CASE);
		predicate = static_cast<Predicate>(option.value.asCase);
	}
	else if (option.key == OPT_TARGET)
	{
		assert(option.value.tag == Option::Value::Type::NUMBER);
		target = option.value.asNumber;
	}
}

bool BlackboardCondition::check()
{
	switch (predicate)
	{
	case Predicate::EXIST:
		return blackboard.has(key);

	case Predicate::EQUAL: 
	{
		float value;
		blackboard.get(key, value);
		return value == target;
	}

	case Predicate::LESS_THAN:
	{
		float value;
		blackboard.get(key, value);
		return value < target;
	}

	case Predicate::GREATER_THAN:
	{
		float value;
		blackboard.get(key, value);
		return value > target;
	}
	default:
		return false;
	}
}
