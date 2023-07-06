// pineBT.cpp : Defines the entry point for the application.
//

#include "pineBT.h"

#include <memory>
#include <API.h>

using namespace std;
using namespace pineBT;

int main()
{
	BehaviorSchemaLibrary schemaLibrary;
	printf("[BehaviorSchemaLibrary]\n%s\n", schemaLibrary.toJSON(2).c_str());

	using Result = Behavior::Result;
	using Mode = Condition::Mode;
	using Logic = Condition::Logic;
	using Predicate = BlackboardCondition::Predicate;

	auto allocator = make_unique<LinearAllocator>(2048);
	auto blackboard = make_unique<Blackboard>();
	Blackboard::Key BB_KEY_A = 1;

	auto condition1a = allocator->allocate<MockCondition>();
	auto condition1b = allocator->allocate<MockCondition>();
	auto task1 = allocator->allocate<MockTask>(Result::SUCCESS);

	auto condition2 = allocator->allocate<MockCondition>();
	auto task2_1 = allocator->allocate<MockTask>(Result::SUCCESS);
	auto task2_2 = allocator->allocate<MockTask>(Result::FAILURE);
	auto task2_3 = allocator->allocate<MockTask>(Result::SUCCESS);

	auto condition3 = allocator->allocate<MockCondition>();
	auto task3 = allocator->allocate<MockTask>(Result::SUCCESS);

	auto condition4 = allocator->allocate<MockCondition>();
	auto task4_1 = allocator->allocate<MockTask>(Result::RUNNING);
	auto task4_2 = allocator->allocate<MockTask>(Result::RUNNING);

	auto condition5 = allocator->allocate<MockCondition>();
	auto condition5a = allocator->allocate<MockCondition>(Mode::CONTINUOUS);
	auto condition5b = allocator->allocate<MockCondition>(Mode::CONTINUOUS, Logic::NEGATE);
	auto task5_1 = allocator->allocate<MockTask>(Result::FAILURE);
	auto task5_2 = allocator->allocate<MockTask>(Result::RUNNING);

	auto condition6 = allocator->allocate<BlackboardCondition>(*blackboard);
	condition6->set(BB_KEY_A, Predicate::GREATER_THAN, 42.17f);
	auto task6 = allocator->allocate<MockTask>(Result::SUCCESS);

	auto behaviorTree = BehaviorTree::build(*allocator, *blackboard)
		.selectLive()
			.condition(condition1a).condition(condition1b)
				.task(task1)
			
			.condition(condition2)
				.sequence()
					.task(task2_1)
					.task(task2_2)
					.task(task2_3)
				.close()

			.condition(condition3)
				.task(task3)

			.condition(condition4)
				.parallel().succeedWhenAny().failWhenAll()
					.task(task4_1)
					.task(task4_2)
				.close()

			.condition(condition5)
				.monitor()
					.condition(condition5a)
					.condition(condition5b)
					.close()
					.select()
						.task(task5_1)
						.task(task5_2)
					.close()
				.close()

			.condition(condition6)
				.task(task6)
		.close()
	.end();

	printf("Run 1\n");
	behaviorTree->run();
	behaviorTree->print();

	printf("\nRun 2\n");
	{
		condition5->setForceCheck(true);
		condition5a->setForceCheck(true);
		condition5b->setForceCheck(false);
	}
	behaviorTree->run();
	behaviorTree->print();

	printf("\nRun 3\n");
	{
		blackboard->set(BB_KEY_A, 87.f);
		condition5b->setForceCheck(true);
		//task5_2->setForceResult(Result::SUCCESS);
	}
	behaviorTree->run();
	behaviorTree->print();

	std::string serialized = R"(
	{
		"root" : {
			"schema" : "Selector",
			"id" : 1,
			"options" : {
				"live" : true
			},
			"children" : [
				{ 
					"schema" : "MockCondition", 
					"id" : 11,
					"children" : [ { 
						"schema" : "MockTask",
						"id" : 111
					} ] 
				},
				{
					"schema" : "Parallel",
					"id" : 12,
					"children" : [
						{ "schema" : "MockTask", "id" : 121 },
						{ "schema" : "MockTask", "id" : 122 }
					]
				}
			]
		}
	}
	)";

	auto handle = pineBT_create(serialized.c_str());
	char* buffer = new char[1024];
	memset(buffer, 0, 1024);
	pineBT_status(handle, buffer);
	printf("TESTTESTESTEST\n%s\n", buffer);

	//LinearAllocator deserializedAllocator(2048);
	//BehaviorTree deserializedBehaviorTree(deserializedAllocator);

	//try
	//{
	//	serialization::JSON::deserialize(serialized, schemaLibrary, deserializedBehaviorTree);
	//	printf("\n - DESERIALIZED -\n");
	//	deserializedBehaviorTree.print();
	//	printf("- DESERIALIZED -\n");
	//} 
	//catch (exception& e)
	//{
	//	printf("[ERROR] %s\n", e.what());
	//}

	return 0;
}
