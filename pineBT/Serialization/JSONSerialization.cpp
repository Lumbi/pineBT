#include "JSONSerialization.h"

using namespace pineBT;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <stack>
#include <assert.h>

static void JSON_deserializeBehaviorRecursively(
	const json& blueprint,
	const BehaviorSchemaLibrary& library,
	BehaviorTree& tree, 
	Behavior* parent
)
{
	auto schema = blueprint.at("schema").get<std::string>();
	auto behavior = library.create(schema, tree);
	assert(behavior);

	if (blueprint.contains("id"))
	{
		behavior->id = blueprint.at("id").get<int>();
	}

	if (blueprint.contains("options"))
	{
		using Option = Behavior::Option;
		using Value = Option::Value;

		const json& options = blueprint.at("options");
		for (auto& [key, value] : options.items())
		{
			if (value.is_boolean())
				behavior->configure(Option::from<Value::Type::BOOLEAN>(key, value.get<bool>()));
			else if (value.is_number())
				behavior->configure(Option::from<Value::Type::NUMBER>(key, value.get<float>()));
			else if (value.contains("case"))
				behavior->configure(Option::from<Value::Type::CASE>(key, value.at("case").get<int>()));
		}
	}

	Behavior* next = behavior.get();

	if (parent == nullptr)
		tree.setRoot(std::move(behavior));
	else
		parent->addChild(std::move(behavior));

	if (blueprint.contains("children"))
	{
		const json& children = blueprint.at("children");
		if (children.is_array() && !children.empty())
		{
			for (auto&& child : children)
			{
				JSON_deserializeBehaviorRecursively(child, library, tree, next);
			}
		}
	}
}

void serialization::JSON::deserialize(
	const std::string& string, 
	const BehaviorSchemaLibrary& library,
	BehaviorTree& tree
) 
{
	json parsed = json::parse(string);
	JSON_deserializeBehaviorRecursively(parsed.at("root"), library, tree, nullptr);
}
