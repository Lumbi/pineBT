#pragma once

namespace pineBT
{
	struct BehaviorTreeBuilderContext
	{
		class BehaviorTreeBuilder& builder;

		BehaviorTreeBuilderContext(BehaviorTreeBuilder& builder) : builder(builder) {}

		virtual void addChild(class Behavior*) = 0;
	};

	struct CompositeContext : public BehaviorTreeBuilderContext
	{
		class Composite* composite;

		CompositeContext(BehaviorTreeBuilder& builder, Composite* composite) 
			: BehaviorTreeBuilderContext(builder), composite(composite) {}

		void addChild(class Behavior*) override;
	};

	struct DecoratorContext : public BehaviorTreeBuilderContext
	{
		class Decorator* decorator;

		DecoratorContext(BehaviorTreeBuilder& builder, Decorator* decorator) 
			: BehaviorTreeBuilderContext(builder), decorator(decorator) {}

		void addChild(class Behavior*) override;
	};
}