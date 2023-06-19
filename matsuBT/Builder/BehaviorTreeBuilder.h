#pragma once

#include "SelectorBuilder.h"

#include <memory>
#include <stack>

namespace matsuBT
{
	class BehaviorTreeBuilder
	{
		friend class SelectorBuilder;

	public:
		BehaviorTreeBuilder();

		SelectorBuilder select();

		std::unique_ptr<class BehaviorTree> end();

	private:
		BehaviorTree* behaviorTree;
		std::stack<class Behavior*> behaviors;
	};
}
