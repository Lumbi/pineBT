#include "MockTask.h"

#include <assert.h>

using namespace pineBT;

static const char* OPT_FORCE_RESULT = "force_result";
static const int OPT_FORCE_RESULT_COUNT = 4;

BehaviorSchema MockTask::schema = BehaviorSchema::inherit(
	"MockTask",
	Task::schema,
	BehaviorSchema::Options().build()
		.enumeration(OPT_FORCE_RESULT, OPT_FORCE_RESULT_COUNT)
	.end()
);

void MockTask::configure(const Option& option)
{
	if (option.key == OPT_FORCE_RESULT)
	{
		assert(option.value.tag == Option::Value::Type::CASE);
		setForceResult(static_cast<Result>(option.value.asCase));
	}
}

Behavior::Result MockTask::update()
{
	return forceResult;
}

void MockTask::setForceResult(Result forceResult)
{
	this->forceResult = forceResult;
}
