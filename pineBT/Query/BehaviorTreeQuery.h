#pragma once

#include "Behavior.h"

#include <unordered_map>

namespace pineBT
{
	struct BehaviorTreeQuery
	{
	public:
		BehaviorTreeQuery(const class BehaviorTree& tree) : tree(tree) {};

		std::unordered_map<Behavior::ID, Behavior::Result> resultsByBehaviorID() const;

	private:
		const BehaviorTree& tree;
	};
}
