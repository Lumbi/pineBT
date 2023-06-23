#pragma once

#include "BehaviorSchema.h"

#include <string>
#include <unordered_map>
#include <assert.h>

namespace pineBT
{
	class BehaviorSchemaLibrary
	{
	public:
		BehaviorSchemaLibrary();

		template<typename BehaviorType>
		void add()
		{
			std::string name = BehaviorType::schema.name;
			assert(schemas.find(name) == schemas.end());
			schemas[name] = BehaviorType::schema;
		}

	private:
		std::unordered_map<std::string, BehaviorSchema> schemas;
	};
}