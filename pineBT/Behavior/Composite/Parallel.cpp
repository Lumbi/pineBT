#include "Parallel.h"

using namespace pineBT;

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
