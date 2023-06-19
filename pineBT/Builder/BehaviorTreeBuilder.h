#pragma once

#include <memory>
#include <stack>

namespace pineBT
{
	class BehaviorTreeBuilder
	{
	public:
		BehaviorTreeBuilder();

		BehaviorTreeBuilder& behavior(std::unique_ptr<class Behavior>);

		BehaviorTreeBuilder& behavior(std::unique_ptr<class Composite>);

		BehaviorTreeBuilder& select();

		BehaviorTreeBuilder& sequence();

		BehaviorTreeBuilder& close();

		BehaviorTreeBuilder& task(std::unique_ptr<class Task> task);

		std::unique_ptr<class BehaviorTree> end();

	private:
		std::unique_ptr<class BehaviorTree> behaviorTree;
		std::stack<class Behavior*> behaviors;
	};
}
