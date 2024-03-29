#include "Sequence.h"

#include <assert.h>

using namespace pineBT;

BehaviorSchema Sequence::schema = BehaviorSchema::inherit(
	"Sequence", 
	Composite::schema
);

void Sequence::configure(const Option&)
{
}

void Sequence::onEnter()
{
	currentChild = children.begin();
}

Behavior::Result Sequence::update()
{
	assert(currentChild != children.end());

	while (true)
	{
		const Result childResult = (*currentChild)->run();

		if (childResult != Result::SUCCESS)
			return childResult;

		if (++currentChild == children.end())
			return Result::SUCCESS;
	}

	return Result::INVALID;
}
