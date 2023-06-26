#include "MockCondition.h"

#include <assert.h>

using namespace pineBT;

static const char* OPT_FORCE_CHECK = "force_check";

BehaviorSchema MockCondition::schema = BehaviorSchema::inherit(
	"MockCondition",
	Condition::schema,
	BehaviorSchema::Options().build()
		.boolean(OPT_FORCE_CHECK)
	.end()
);

void pineBT::MockCondition::configure(const Option& option)
{
	if (option.key == OPT_FORCE_CHECK)
	{
		assert(option.value.tag == Option::Value::Type::BOOLEAN);
		setForceCheck(option.value.asBoolean);
	}
}

bool MockCondition::check()
{
	return forceCheck;
}
