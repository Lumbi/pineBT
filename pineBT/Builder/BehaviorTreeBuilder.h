#pragma once

#include <memory>
#include <stack>

namespace pineBT
{
	class BehaviorTreeBuilder
	{
	public:
		BehaviorTreeBuilder();

		BehaviorTreeBuilder& behavior(class Composite*);

		BehaviorTreeBuilder& behavior(class Decorator*);

		BehaviorTreeBuilder& select();

		BehaviorTreeBuilder& selectLive();

		BehaviorTreeBuilder& sequence();

		BehaviorTreeBuilder& close();

		BehaviorTreeBuilder& conditon(class Condition*);

		BehaviorTreeBuilder& task(class Task*);

		std::unique_ptr<class BehaviorTree> end();
		 
	private:
		std::unique_ptr<class BehaviorTree> behaviorTree;

	private:
		struct Context
		{
			BehaviorTreeBuilder& builder;

			Context(BehaviorTreeBuilder& builder) : builder(builder) {}

			virtual void addChild(class Behavior*) = 0;
		};

		struct CompositeContext : public Context
		{
			class Composite* composite;

			CompositeContext(BehaviorTreeBuilder& builder, Composite* composite) : Context(builder), composite(composite) {}

			void addChild(class Behavior*) override;
		};

		struct DecoratorContext : public Context
		{
			class Decorator* decorator;

			DecoratorContext(BehaviorTreeBuilder& builder, Decorator* decorator) : Context(builder), decorator(decorator) {}
			
			void addChild(class Behavior*) override;
		};

		std::stack<std::unique_ptr<Context>> context;
	};
}
