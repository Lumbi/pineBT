#include "Parallel.h"

#include <assert.h>

using namespace pineBT;

const char* OPT_POLICY_SUCCESS = "success_policy";
const char* OPT_POLICY_FAILURE = "failure_policy";
const static int OPT_POLICY_COUNT = 2;

BehaviorSchema Parallel::schema = BehaviorSchema::inherit(
	"Parallel",
	Composite::schema,
	BehaviorSchema::Options().build()
		.enumeration(OPT_POLICY_SUCCESS, OPT_POLICY_COUNT)
		.enumeration(OPT_POLICY_FAILURE, OPT_POLICY_COUNT)
	.end()
);

void Parallel::configure(const Option& option)
{
	if (option.key == OPT_POLICY_SUCCESS)
	{
		assert(option.value.tag == Option::Value::Type::CASE);
		successPolicy = static_cast<Policy>(option.value.asCase);
	}
	else if (option.key == OPT_POLICY_FAILURE)
	{
		assert(option.value.tag == Option::Value::Type::CASE);
		failurePolicy = static_cast<Policy>(option.value.asCase);
	}
}

Behavior::Result Parallel::update()
{
	int successCount = 0;
	int failureCount = 0;
	for (auto&& child : children)
	{
		if (!child->isCompleted()) child->run();
		const Result result = child->getResult();
		if (result == Result::SUCCESS)
		{
			if (successPolicy == Policy::ANY) return Result::SUCCESS;
			successCount++;
		}
		if (result == Result::FAILURE)
		{
			if (failurePolicy == Policy::ANY) return Result::FAILURE;
			failureCount++;
		}
	}

	if (failurePolicy == Policy::ALL && failureCount == children.size())
		return Result::FAILURE;
	if (successPolicy == Policy::ALL && successCount == children.size())
		return Result::SUCCESS;

	return Result::RUNNING;
}

void Parallel::onExit(Result)
{
	for (auto&& child : children)
	{
		child->abort();
	}
}
