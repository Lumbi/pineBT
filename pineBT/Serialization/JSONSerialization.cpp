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
	Behavior* behavior = library.create(schema, tree);
	assert(behavior);

	// TODO: configure

	if (parent == nullptr)
		tree.setRoot(behavior);
	else
		parent->addChild(behavior);

	if (blueprint.contains("children"))
	{
		const json& children = blueprint.at("children");
		if (children.is_array() && !children.empty())
		{
			for (auto&& child : children)
			{
				JSON_deserializeBehaviorRecursively(child, library, tree, behavior);
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
