#pragma once

#include <memory>
#include <stack>

namespace pineBT
{
	class BehaviorTreeBuilder
	{
	public:
		BehaviorTreeBuilder();

		BehaviorTreeBuilder& behavior(std::unique_ptr<class Composite>);

		BehaviorTreeBuilder& behavior(std::unique_ptr<class Decorator>);

		BehaviorTreeBuilder& select();

		BehaviorTreeBuilder& sequence();

		BehaviorTreeBuilder& close();

		BehaviorTreeBuilder& conditon(std::unique_ptr<class Condition>);

		BehaviorTreeBuilder& task(std::unique_ptr<class Task> task);

		std::unique_ptr<class BehaviorTree> end();

	private:
		std::unique_ptr<class BehaviorTree> behaviorTree;

	private:
		struct Context
		{
			BehaviorTreeBuilder& builder;

			Context(BehaviorTreeBuilder& builder) : builder(builder) {}

			virtual void addChild(std::unique_ptr<class Behavior>) = 0;
		};

		struct CompositeContext : public Context
		{
			class Composite* composite;

			CompositeContext(BehaviorTreeBuilder& builder, Composite* composite) : Context(builder), composite(composite) {}

			void addChild(std::unique_ptr<class Behavior> child) override;
		};

		struct DecoratorContext : public Context
		{
			class Decorator* decorator;

			DecoratorContext(BehaviorTreeBuilder& builder, Decorator* decorator) : Context(builder), decorator(decorator) {}
			
			void addChild(std::unique_ptr<class Behavior>) override;
		};

		std::stack<std::unique_ptr<Context>> context;
	};
}
