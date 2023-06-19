#include "BehaviorTreeBuilder.h"

#include "BehaviorTree.h"
#include "Behavior.h"
#include "Behavior/Composite/Selector.h"

using namespace matsuBT;

SelectorBuilder BehaviorTreeBuilder::select()
{
	auto selector = std::make_unique<Selector>();

	behaviors.push(selector.get());

	if (!behaviorTree->getRoot()) 
		behaviorTree->setRoot(std::move(selector));

	return SelectorBuilder(*this);
}

std::unique_ptr<BehaviorTree> BehaviorTreeBuilder::end()
{
	return std::unique_ptr<BehaviorTree>(behaviorTree);
}

BehaviorTreeBuilder::BehaviorTreeBuilder()
	: behaviorTree(new BehaviorTree())
{
}
