#include "Sequence.h"

using namespace matsuBT;

Behavior::Result Sequence::run()
{
	for (auto&& child : children)
	{
		const Result result = child->run();
		if (result == Result::SUCCESS)
			continue;
		else
			return result;
	}
	return Result::SUCCESS;
}
