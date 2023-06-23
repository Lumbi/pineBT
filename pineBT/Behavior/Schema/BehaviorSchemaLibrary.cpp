#include "BehaviorSchemaLibrary.h"

#include "Behavior/Composite/Selector.h"
#include "Behavior/Composite/Sequence.h"
#include "Behavior/Composite/Parallel.h"

#include "Behavior/Mock/MockCondition.h"
#include "Behavior/Mock/MockTask.h"

using namespace pineBT;

BehaviorSchemaLibrary::BehaviorSchemaLibrary()
{
	add<Selector>();
	add<Sequence>();
	add<Parallel>();

	add<MockCondition>();
	add<MockTask>();
}
