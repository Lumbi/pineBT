#include "BehaviorTree.h"

#include "Behavior.h"

#include <assert.h>

using namespace pineBT;

BehaviorTree::BehaviorTree()
	: root(nullptr)
{
}

BehaviorTreeBuilder BehaviorTree::build()
{
	return BehaviorTreeBuilder();
}

void BehaviorTree::setRoot(std::unique_ptr<Behavior> newRoot)
{
	root = std::move(newRoot);
}

Behavior* BehaviorTree::getRoot() const
{
	return root.get();
}

void BehaviorTree::run()
{
	assert(root != nullptr);

	root->run();
}
