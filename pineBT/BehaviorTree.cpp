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

void BehaviorTree::setRoot(Behavior* newRoot)
{
	root = newRoot;
}

Behavior* BehaviorTree::getRoot() const
{
	return root;
}

void BehaviorTree::run()
{
	assert(root != nullptr);

	root->run();
}
