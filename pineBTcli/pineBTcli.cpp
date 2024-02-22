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

	auto blackboard = make_unique<Blackboard>();
	Blackboard::Key BB_KEY_A = 1;

	auto condition = std::make_unique<BlackboardCondition>(*blackboard);
	condition->set(BB_KEY_A, Predicate::GREATER_THAN, 42.17f);

	auto behaviorTree = BehaviorTree::build(*blackboard)
		.selectLive()
			.condition(std::make_unique<MockCondition>()).condition(std::make_unique<MockCondition>())
				.task(std::make_unique<MockTask>(Result::SUCCESS))

			.condition(std::make_unique<MockCondition>())
				.sequence()
					.task(std::make_unique<MockTask>(Result::SUCCESS))
					.task(std::make_unique<MockTask>(Result::FAILURE))
					.task(std::make_unique<MockTask>(Result::SUCCESS))
				.close()

			.condition(std::make_unique<MockCondition>())
				.task(std::make_unique<MockTask>(Result::SUCCESS))

			.condition(std::make_unique<MockCondition>())
				.parallel().succeedWhenAny().failWhenAll()
					.task(std::make_unique<MockTask>(Result::RUNNING))
					.task(std::make_unique<MockTask>(Result::RUNNING))
				.close()

			.condition(std::make_unique<MockCondition>())
				.monitor()
					.condition(std::make_unique<MockCondition>(Mode::CONTINUOUS))
					.condition(std::make_unique<MockCondition>(Mode::CONTINUOUS, Logic::NEGATE))
					.close()
					.select()
						.task(std::make_unique<MockTask>(Result::FAILURE))
						.task(std::make_unique<MockTask>(Result::RUNNING))
					.close()
				.close()

			.condition(std::move(condition))
				.task(std::make_unique<MockTask>(Result::SUCCESS))
		.close()
	.end();

	printf("Run 1\n");
	behaviorTree->run();
	behaviorTree->print();

	//printf("\nRun 2\n");
	//{
	//	condition5->setForceCheck(true);
	//	condition5a->setForceCheck(true);
	//	condition5b->setForceCheck(false);
	//}
	//behaviorTree->run();
	//behaviorTree->print();

	//printf("\nRun 3\n");
	//{
	//	blackboard->set(BB_KEY_A, 87.f);
	//	condition5b->setForceCheck(true);
	//}
	//behaviorTree->run();
	//behaviorTree->print();

	return 0;
}
