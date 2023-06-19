// pineBT.cpp : Defines the entry point for the application.
//

#include "pineBT.h"

#include <memory>

using namespace std;
using namespace pineBT;

int main()
{
	auto behaviorTree = BehaviorTree::build()
		.select()
			.task(std::make_unique<DebugTask>("test task 1", Behavior::Result::FAILURE))
			.sequence()
				.task(std::make_unique<DebugTask>("test task 1.1"))
				.task(std::make_unique<DebugTask>("test task 1.2", Behavior::Result::FAILURE))
				.task(std::make_unique<DebugTask>("test task 1.3"))
			.close()
			.task(std::make_unique<DebugTask>("test task 2"))
		.close()
	.end();

	behaviorTree->run();

	return 0;
}
