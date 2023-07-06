#include "BehaviorTreeBuilder.h"

#include "Memory/LinearAllocator.h"
#include "BehaviorTree.h"
#include "Behavior.h"
#include "Blackboard/Blackboard.h"
#include "Behavior/Composite/Composite.h"
#include "Behavior/Composite/Selector.h"
#include "Behavior/Composite/Sequence.h"
#include "Behavior/Composite/Parallel.h"
#include "Behavior/Composite/Monitor.h"
#include "Behavior/Decorator/Decorator.h"
#include "Behavior/Decorator/Condition.h"
#include "Behavior/Task/Task.h"

#include <assert.h>

using namespace pineBT;

BehaviorTreeBuilder::BehaviorTreeBuilder(LinearAllocator& allocator, const Blackboard& blackboard)
	: behaviorTree(std::make_unique<BehaviorTree>(allocator, blackboard))
{
}

BehaviorTreeBuilder& BehaviorTreeBuilder::behavior(Composite* composite)
{
	addChild(composite);
	auto nextContext = std::make_unique<CompositeContext>(*this, composite);
	context.push(std::move(nextContext));
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::behavior(Decorator* decorator)
{
	addChild(decorator);
	auto nextContext = std::make_unique<DecoratorContext>(*this, decorator);
	context.push(std::move(nextContext));
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::select()
{
	return this->behavior(behaviorTree->getAllocator().allocate<Selector>());
}

BehaviorTreeBuilder& BehaviorTreeBuilder::selectLive()
{
	auto selector = behaviorTree->getAllocator().allocate<Selector>();
	selector->setLive(true);
	return this->behavior(selector);
}

BehaviorTreeBuilder& BehaviorTreeBuilder::sequence()
{
	return this->behavior(behaviorTree->getAllocator().allocate<Sequence>());
}

ParallelSuccessPolicyBuilder BehaviorTreeBuilder::parallel()
{
	Parallel* parallel = behaviorTree->getAllocator().allocate<Parallel>();
	this->behavior(parallel);
	return ParallelSuccessPolicyBuilder(*this, parallel);
}

BehaviorTreeBuilder& BehaviorTreeBuilder::monitor()
{
	Monitor* monitor = behaviorTree->getAllocator().allocate<Monitor>();
	this->behavior(monitor);
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::close()
{
	assert(!context.empty());
	context.pop();
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::condition(Condition* condition)
{
	return this->behavior(condition);
}

BehaviorTreeBuilder& BehaviorTreeBuilder::task(Task* task)
{
	assert(!context.empty());
	context.top()->addChild(task);
	return *this;
}

std::unique_ptr<BehaviorTree> BehaviorTreeBuilder::end()
{
	assert(context.empty());
	return std::move(behaviorTree);
}

void BehaviorTreeBuilder::addChild(Behavior* child)
{
	if (context.empty())
	{
		behaviorTree->setRoot(child);
	}
	else
	{
		context.top()->addChild(child);
	}
}
