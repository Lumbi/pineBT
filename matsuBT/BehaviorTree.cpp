#include "BehaviorTree.h"

using namespace matsuBT;

BehaviorTree::BehaviorTree()
	: root(nullptr)
{
}

void BehaviorTree::setRoot(std::unique_ptr<Behavior> newRoot)
{
	root = std::move(newRoot);
}
