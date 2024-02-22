#include "BehaviorTreeBuilder.h"

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

BehaviorTreeBuilder::BehaviorTreeBuilder(const Blackboard& blackboard)
	: behaviorTree(std::make_unique<BehaviorTree>(blackboard))
{
}

BehaviorTreeBuilder& BehaviorTreeBuilder::behavior(std::unique_ptr<Composite> composite)
{
	auto nextContext = std::make_unique<CompositeContext>(*this, composite.get());
	addChild(std::move(composite));
	context.push(std::move(nextContext));
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::behavior(std::unique_ptr<Decorator> decorator)
{
	auto nextContext = std::make_unique<DecoratorContext>(*this, decorator.get());
	addChild(std::move(decorator));
	context.push(std::move(nextContext));
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::select()
{
	return this->behavior(std::make_unique<Selector>());
}

BehaviorTreeBuilder& BehaviorTreeBuilder::selectLive()
{
	auto selector = std::make_unique<Selector>();
	selector->setLive(true);
	return this->behavior(std::move(selector));
}

BehaviorTreeBuilder& BehaviorTreeBuilder::sequence()
{
	return this->behavior(std::make_unique<Sequence>());
}

ParallelSuccessPolicyBuilder BehaviorTreeBuilder::parallel()
{
	auto parallel = std::make_unique<Parallel>();
	auto policyBuilder = ParallelSuccessPolicyBuilder(*this, parallel.get());
	this->behavior(std::move(parallel));
	return policyBuilder;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::monitor()
{
	return this->behavior(std::make_unique<Monitor>());
}

BehaviorTreeBuilder& BehaviorTreeBuilder::close()
{
	assert(!context.empty());
	context.pop();
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::condition(std::unique_ptr<Condition> condition)
{
	return this->behavior(std::move(condition));
}

BehaviorTreeBuilder& BehaviorTreeBuilder::task(std::unique_ptr<Task> task)
{
	assert(!context.empty());
	context.top()->addChild(std::move(task));
	return *this;
}

std::unique_ptr<BehaviorTree> BehaviorTreeBuilder::end()
{
	assert(context.empty());
	return std::move(behaviorTree);
}

void BehaviorTreeBuilder::addChild(std::unique_ptr<Behavior> child)
{
	if (context.empty())
	{
		behaviorTree->setRoot(std::move(child));
	}
	else
	{
		context.top()->addChild(std::move(child));
	}
}
