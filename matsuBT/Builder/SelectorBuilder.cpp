#include "SelectorBuilder.h"

#include "BehaviorTreeBuilder.h"
#include "Behavior.h"
#include "Behavior/Composite/Selector.h"
#include "Behavior/Task/Task.h"

#include <assert.h>

using namespace matsuBT;

SelectorBuilder& SelectorBuilder::task(std::unique_ptr<Task> task)
{
	assert(!builder.behaviors.empty());

	auto selector = static_cast<Selector*>(builder.behaviors.top());
	selector->addChild(std::move(task));
	return *this;
}

BehaviorTreeBuilder& SelectorBuilder::end()
{
	builder.behaviors.pop();
	return builder;
}
