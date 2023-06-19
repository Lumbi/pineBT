#include "Sequence.h"

using namespace matsuBT;

void Sequence::onEnter()
{
	currentChild = children.begin();
}

Behavior::Result Sequence::run()
{
#if DEBUG
	if (currentChild == children.end()) { return Result::INVALID; }
#endif

	while (true)
	{
		const Result result = (*currentChild)->run();

		if (result != Result::SUCCESS)
			return result;

		if (++currentChild == children.end())
			return Result::SUCCESS;
	}

	return Result::INVALID;
}
