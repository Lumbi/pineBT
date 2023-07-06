#include "API.h"

#include "BehaviorTree.h"
#include "Blackboard/Blackboard.h"
#include "Query/BehaviorTreeQuery.h"
#include "Memory/LinearAllocator.h"
#include "Behavior/Schema/BehaviorSchemaLibrary.h"
#include "Serialization/JSONSerialization.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

using namespace pineBT;

static std::unordered_map<BehaviorTreeHandle, std::unique_ptr<BehaviorTree>> trees;
static std::unordered_map<BehaviorTreeHandle, std::unique_ptr<Blackboard>> blackboards;
static LinearAllocator allocator(64'000);
static BehaviorSchemaLibrary schemaLibrary;

static BehaviorTreeHandle nextHandle()
{
	static BehaviorTreeHandle handle = 0;
	return ++handle;
}

void pineBT_schemas(char* buffer)
{
	const std::string dump = schemaLibrary.toJSON();
	memcpy(buffer, dump.c_str(), dump.size());
}

BehaviorTreeHandle pineBT_create(const char* json)
{
	auto blackboard = std::make_unique<Blackboard>();
	auto behaviorTree = std::make_unique<BehaviorTree>(allocator, *blackboard);
	try
	{
		serialization::JSON::deserialize(json, schemaLibrary, *behaviorTree);
		const BehaviorTreeHandle handle = nextHandle();
		trees[handle] = std::move(behaviorTree);
		blackboards[handle] = std::move(blackboard);
		return handle;
	}
	catch (std::exception& e)
	{
		printf("%s\n", e.what());
		return 0;
	}
}

void pineBT_destroy(BehaviorTreeHandle handle)
{
	trees.erase(handle);
	blackboards.erase(handle);
}

void pineBT_run(BehaviorTreeHandle handle)
{
	trees[handle]->run();
}

void pineBT_status(BehaviorTreeHandle handle, char* buffer)
{
	using json = nlohmann::json;

	BehaviorTreeQuery query(*trees[handle]);
	auto results = query.resultsByBehaviorID();

	json resultsJSON;
	for (auto& [behaviorID, result] : results)
	{
		resultsJSON.push_back({
			{ "id", behaviorID },
			{ "status", name(result) }
		});
	}

	const std::string dump = resultsJSON.dump();
	memcpy(buffer, dump.c_str(), dump.size());
}

void pineBT_blackboard_setf(BehaviorTreeHandle handle, Blackboard::Key key, float value)
{
	blackboards[handle]->set(key, value);
}

void pineBT_blackboard_clear(BehaviorTreeHandle handle, Blackboard::Key key)
{
	blackboards[handle]->clear(key);
}
