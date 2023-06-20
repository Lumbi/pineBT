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

BehaviorTreeBuilder& BehaviorTreeBuilder::behavior(std::unique_ptr<Composite> composite)
{
	auto nextContext = std::make_unique<CompositeContext>(*this, composite.get());
	if (context.empty())
	{
		behaviorTree->setRoot(std::move(composite));
	}
	else
	{
		context.top()->addChild(std::move(composite));
	}
	context.push(std::move(nextContext));
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::behavior(std::unique_ptr<Decorator> decorator)
{
	auto nextContext = std::make_unique<DecoratorContext>(*this, decorator.get());
	if (context.empty())
	{
		behaviorTree->setRoot(std::move(decorator));
	}
	else
	{
		context.top()->addChild(std::move(decorator));
	}
	context.push(std::move(nextContext));
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::select()
{
	return this->behavior(std::make_unique<Selector>());
}

BehaviorTreeBuilder& BehaviorTreeBuilder::sequence()
{
	return this->behavior(std::make_unique<Sequence>());
}

BehaviorTreeBuilder& BehaviorTreeBuilder::close()
{
	assert(!context.empty());
	context.pop();
	return *this;
}

BehaviorTreeBuilder& BehaviorTreeBuilder::conditon(std::unique_ptr<Condition> condition)
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

// Builder Context

void BehaviorTreeBuilder::CompositeContext::addChild(std::unique_ptr<Behavior> child)
{
	composite->addChild(std::move(child));
}

void BehaviorTreeBuilder::DecoratorContext::addChild(std::unique_ptr<Behavior> child)
{
	decorator->setChild(std::move(child));
	builder.context.pop();
}
