#pragma once

#include "Behavior.h"
#include "BehaviorTree.h"
#include "Behavior/Schema/BehaviorSchemaLibrary.h"

#include <string>

namespace pineBT
{
	namespace serialization
	{
		namespace JSON
		{
			void deserialize(
				const std::string&,
				const BehaviorSchemaLibrary&,
				BehaviorTree&
			);
		}
	}
}
