#include "MockTask.h"

using namespace pineBT;

BehaviorSchema MockTask::schema = BehaviorSchema::inherit(
	"MockTask",
	Task::schema,
	BehaviorSchema::Options().build()
		.enumeration("force_result", { "running", "success", "failure", "abort" })
	.end()
);

Behavior::Result MockTask::update()
{
	printf("%s\n", message.c_str());
	return forceResult;
}

void pineBT::MockTask::setMessage(const std::string& message)
{
	this->message = message;
}

void MockTask::setForceResult(Result forceResult)
{
	this->forceResult = forceResult;
}
