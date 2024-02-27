#include "BehaviorTreeQuery.h"

#include "BehaviorTree.h"

#include <assert.h>
#include <stack>

using namespace pineBT;

std::unordered_map<Behavior::ID, Behavior::Result> BehaviorTreeQuery::resultsByBehaviorID() const
{
	assert(tree.getRoot());
	std::unordered_map<Behavior::ID, Behavior::Result> map;
	for (auto&& child : tree) map[child.id] = child.getResult();
	return map;
}
