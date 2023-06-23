#pragma once

#include "Builder/BehaviorTreeBuilderContext.h"
#include "Builder/ParallelBuilder.h"

#include <memory>
#include <stack>

namespace pineBT
{
	struct BehaviorTreeBuilder
	{
	public:
		BehaviorTreeBuilder();

		BehaviorTreeBuilder& behavior(class Composite*);

		BehaviorTreeBuilder& behavior(class Decorator*);

		BehaviorTreeBuilder& select();

		BehaviorTreeBuilder& selectLive();

		BehaviorTreeBuilder& sequence();

		ParallelSuccessPolicyBuilder parallel();

		BehaviorTreeBuilder& monitor();

		BehaviorTreeBuilder& close();

		BehaviorTreeBuilder& condition(class Condition*);

		BehaviorTreeBuilder& task(class Task*);

		std::unique_ptr<class BehaviorTree> end();

	private:
		std::unique_ptr<class BehaviorTree> behaviorTree;
		std::stack<std::unique_ptr<BehaviorTreeBuilderContext>> context;

		void addChild(Behavior*);
	};
}
