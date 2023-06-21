#include "BehaviorTreeBuilder.h"

#include "BehaviorTree.h"
#include "Behavior.h"
#include "Behavior/Composite/Composite.h"
#include "Behavior/Composite/Selector.h"
#include "Behavior/Composite/Sequence.h"
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
	if (context.empty())
	{
		behaviorTree->setRoot(composite);
	}
	else
	{
		context.top()->addChild(composite);
	}
	auto nextContext = std::make_unique<CompositeContext>(*this, composite);
	context.push(std::move(nextContext));
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::behavior(Decorator* decorator)
{
	if (context.empty())
	{
		behaviorTree->setRoot(decorator);
	}
	else
	{
		context.top()->addChild(decorator);
	}
	auto nextContext = std::make_unique<DecoratorContext>(*this, decorator);
	context.push(std::move(nextContext));
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::select()
{
	return this->behavior(behaviorTree->allocate<Selector>());
}

BehaviorTreeBuilder& BehaviorTreeBuilder::sequence()
{
	return this->behavior(behaviorTree->allocate<Sequence>());
}

BehaviorTreeBuilder& BehaviorTreeBuilder::close()
{
	assert(!context.empty());
	context.pop();
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::conditon(Condition* condition)
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

// Builder Context

void BehaviorTreeBuilder::CompositeContext::addChild(Behavior* child)
{
	composite->addChild(child);
}

void BehaviorTreeBuilder::DecoratorContext::addChild(Behavior* child)
{
	decorator->setChild(child);
	builder.context.pop();
}
