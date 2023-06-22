#include "BehaviorTreeBuilder.h"

#include "BehaviorTree.h"
#include "Behavior.h"
#include "Behavior/Composite/Composite.h"
#include "Behavior/Composite/Selector.h"
#include "Behavior/Composite/Sequence.h"
#include "Behavior/Composite/Parallel.h"
#include "Behavior/Decorator/Decorator.h"
#include "Behavior/Decorator/Condition.h"
#include "Behavior/Task/Task.h"

#include <assert.h>

using namespace pineBT;

BehaviorTreeBuilder::BehaviorTreeBuilder()
	: behaviorTree(std::make_unique<BehaviorTree>())
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
	return this->behavior(behaviorTree->allocate<Selector>());
}

BehaviorTreeBuilder& BehaviorTreeBuilder::selectLive()
{
	return this->behavior(behaviorTree->allocate<LiveSelector>());
}

BehaviorTreeBuilder& BehaviorTreeBuilder::sequence()
{
	return this->behavior(behaviorTree->allocate<Sequence>());
}

ParallelSuccessPolicyBuilder BehaviorTreeBuilder::parallel()
{
	Parallel* parallel = behaviorTree->allocate<Parallel>();
	this->behavior(parallel);
	return ParallelSuccessPolicyBuilder(*this, parallel);
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
