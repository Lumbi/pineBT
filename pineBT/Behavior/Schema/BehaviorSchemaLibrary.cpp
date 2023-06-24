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

#include <nlohmann/json.hpp>

std::string BehaviorSchemaLibrary::toJSON(int indent) const
{
	using json = nlohmann::json;
	using Hierarchy = BehaviorSchema::Hierarchy;

	json jsonResult;
	for (auto&& pair : schemas)
	{
		auto schema = pair.second;

		json jsonSchema;
		jsonSchema["name"] = schema.name;

		json& jsonHierarchy = jsonSchema["hierarchy"];
		switch (schema.hierarchy)
		{
		case Hierarchy::NONE: jsonHierarchy = "none"; break;
		case Hierarchy::ONE: jsonHierarchy = "one"; break;
		case Hierarchy::MANY: jsonHierarchy = "many"; break;
		}

		json& jsonOptions = jsonSchema["options"];
		for (auto&& option : schema.options.booleans) 
			jsonOptions[option] = "boolean";

		for (auto&& option : schema.options.numbers) 
			jsonOptions[option] = "number";

		for (auto&& option : schema.options.enums) 
			jsonOptions[option.first] = option.second;

		jsonResult[schema.name] = jsonSchema;
	}

	return jsonResult.dump(indent);
}
