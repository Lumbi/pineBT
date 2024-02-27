#pragma once

#include <memory>

namespace pineBT
{
	struct BehaviorTreeBuilderContext
	{
		struct BehaviorTreeBuilder& builder;

		BehaviorTreeBuilderContext(BehaviorTreeBuilder& builder) : builder(builder) {}
		virtual ~BehaviorTreeBuilderContext() {};

		virtual void addChild(std::unique_ptr<class Behavior>) = 0;
	};

	struct CompositeContext : public BehaviorTreeBuilderContext
	{
		class Composite* composite;

		CompositeContext(BehaviorTreeBuilder& builder, Composite* composite) 
			: BehaviorTreeBuilderContext(builder), composite(composite) {}

		void addChild(std::unique_ptr<class Behavior>) override;
	};

	struct DecoratorContext : public BehaviorTreeBuilderContext
	{
		class Decorator* decorator;

		DecoratorContext(BehaviorTreeBuilder& builder, Decorator* decorator) 
			: BehaviorTreeBuilderContext(builder), decorator(decorator) {}

		void addChild(std::unique_ptr<class Behavior>) override;
	};
}