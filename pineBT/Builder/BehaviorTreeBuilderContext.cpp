#include "BehaviorTreeBuilderContext.h"

#include "BehaviorTreeBuilder.h"
#include "Behavior/Composite/Composite.h"
#include "Behavior/Decorator/Decorator.h"

using namespace pineBT;

void CompositeContext::addChild(Behavior* child)
{
	composite->addChild(child);
}

void DecoratorContext::addChild(Behavior* child)
{
	decorator->setChild(child);
	builder.close();
}
