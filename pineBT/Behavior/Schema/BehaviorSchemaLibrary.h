#pragma once

#include "BehaviorTree.h"
#include "BehaviorSchema.h"
#include "Behavior/Decorator/BlackboardCondition.h"

#include <string>
#include <unordered_map>
#include <functional>
#include <assert.h>

namespace pineBT
{
	class BehaviorSchemaLibrary
	{
	private:
		using BehaviorCreator = std::function<Behavior* (const std::string& name, BehaviorTree& tree)>;

	public:
		BehaviorSchemaLibrary();

		template<typename BehaviorType>
		void add()
		{
			std::string name = BehaviorType::schema.name;
			assert(schemas.find(name) == schemas.end());
			schemas[name] = BehaviorType::schema;
			BehaviorCreator creator = [](const std::string& name, BehaviorTree& tree) {
				return tree.getAllocator().allocate<BehaviorType>();
			};
			creators[name] = creator;
		}

		template<>
		void add<BlackboardCondition>()
		{
			std::string name = BlackboardCondition::schema.name;
			assert(schemas.find(name) == schemas.end());
			schemas[name] = BlackboardCondition::schema;
			BehaviorCreator creator = [](const std::string& name, BehaviorTree& tree) {
				return tree.getAllocator().allocate<BlackboardCondition>(tree.getBlackboard());
			};
			creators[name] = creator;
		}

		Behavior* create(const std::string& name, BehaviorTree& tree) const;

		std::string toJSON(int indent = -1) const;

	private:
		std::unordered_map<std::string, BehaviorSchema> schemas;
		std::unordered_map<std::string, BehaviorCreator> creators;
	};
}