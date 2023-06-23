#include "MockCondition.h"

using namespace pineBT;

BehaviorSchema MockCondition::schema = BehaviorSchema::inherit(
	"MockCondition",
	Condition::schema,
	BehaviorSchema::Options().build()
		.boolean("force_check")
	.end()
);

bool MockCondition::check()
{
	return forceCheck;
}
