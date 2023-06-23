#include "BehaviorTree.h"

#include "Behavior.h"
#include "Printer/BehaviorTreePrinter.h"

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

void BehaviorTree::print()
{
	BehaviorTreePrinter(*this).print();
}

// Iterator support

BehaviorTree::iterator::reference BehaviorTree::iterator::operator*() const
{
	return *pointers.front();
}

BehaviorTree::iterator::pointer BehaviorTree::iterator::operator->()
{
	return pointers.front();
}

BehaviorTree::iterator& BehaviorTree::iterator::operator++()
{
	if (pointers.empty()) return *this;

	pointer current = pointers.front();
	pointers.pop();

	// TODO: Maybe all behaviors should have a child array

	if (auto composite = dynamic_cast<Composite*>(current); composite)
	{
		for (auto&& child : composite->getChildren())
			pointers.push(child);
	}
	else if (auto decorator = dynamic_cast<Decorator*>(current); decorator)
	{
		if (auto child = decorator->getChild(); child)
			pointers.push(child);
	}

	return *this;
}

BehaviorTree::iterator BehaviorTree::iterator::operator++(int)
{
	iterator old = *this; ++(*this); return old;
}

bool pineBT::operator==(const BehaviorTree::iterator& lhs, const BehaviorTree::iterator& rhs)
{
	if (lhs.pointers.empty() && rhs.pointers.empty()) return true;
	if (lhs.pointers.empty() || rhs.pointers.empty()) return false;
	return lhs.pointers.front() == rhs.pointers.front();
}

bool pineBT::operator!=(const BehaviorTree::iterator& lhs, const BehaviorTree::iterator& rhs)
{
	return !(lhs == rhs);
}
