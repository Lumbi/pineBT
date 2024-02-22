#include "BehaviorTreeBuilderContext.h"

#include "BehaviorTreeBuilder.h"
#include "Behavior/Composite/Composite.h"
#include "Behavior/Decorator/Decorator.h"

using namespace pineBT;

void CompositeContext::addChild(std::unique_ptr<Behavior> child)
{
	composite->addChild(std::move(child));
}

void DecoratorContext::addChild(std::unique_ptr<Behavior> child)
{
	decorator->setChild(std::move(child));
	builder.close();
}
