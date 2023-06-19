#include "BehaviorTreeBuilder.h"

#include "BehaviorTree.h"
#include "Behavior.h"
#include "Behavior/Composite/Composite.h"
#include "Behavior/Composite/Selector.h"
#include "Behavior/Composite/Sequence.h"
#include "Behavior/Task/Task.h"

#include <assert.h>

using namespace matsuBT;

BehaviorTreeBuilder::BehaviorTreeBuilder()
	: behaviorTree(std::make_unique<BehaviorTree>())
{
}

BehaviorTreeBuilder& BehaviorTreeBuilder::behavior(std::unique_ptr<Behavior> behavior)
{
	assert(!behaviors.empty());
	
	auto composite = static_cast<Composite*>(behaviors.top());
	if (dynamic_cast<Composite*>(behavior.get())) 
	{
		behaviors.push(behavior.get());
	}
	composite->addChild(std::move(behavior));

	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::behavior(std::unique_ptr<Composite> composite)
{
	// Set root if not set
	if (behaviors.empty())
	{
		behaviors.push(composite.get());
		behaviorTree->setRoot(std::move(composite));
	}
	// Add as child to current composite behavior
	else
	{
		auto parent = static_cast<Composite*>(behaviors.top());
		behaviors.push(composite.get());
		parent->addChild(std::move(composite));
	}

	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::select()
{
	std::unique_ptr<Composite> selector = std::make_unique<Selector>();
	return this->behavior(std::move(selector));
}

BehaviorTreeBuilder& BehaviorTreeBuilder::sequence()
{
	std::unique_ptr<Composite> sequence = std::make_unique<Sequence>();
	return this->behavior(std::move(sequence));
}

BehaviorTreeBuilder& BehaviorTreeBuilder::close()
{
	assert(!behaviors.empty());
	behaviors.pop();
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::task(std::unique_ptr<Task> task)
{
	return this->behavior(std::move(task));
}

std::unique_ptr<BehaviorTree> BehaviorTreeBuilder::end()
{
	return std::move(behaviorTree);
}
