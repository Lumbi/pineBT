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
		BehaviorTreeBuilder(const class Blackboard&);

		BehaviorTreeBuilder& behavior(std::unique_ptr<class Composite>);

		BehaviorTreeBuilder& behavior(std::unique_ptr<class Decorator>);

		BehaviorTreeBuilder& select();

		BehaviorTreeBuilder& selectLive();

		BehaviorTreeBuilder& sequence();

		ParallelSuccessPolicyBuilder parallel();

		BehaviorTreeBuilder& monitor();

		BehaviorTreeBuilder& close();

		BehaviorTreeBuilder& condition(std::unique_ptr<class Condition>);

		BehaviorTreeBuilder& task(std::unique_ptr<class Task>);

		std::unique_ptr<class BehaviorTree> end();

	private:
		std::unique_ptr<class BehaviorTree> behaviorTree;
		std::stack<std::unique_ptr<BehaviorTreeBuilderContext>> context;

		void addChild(std::unique_ptr<Behavior>);
	};
}
