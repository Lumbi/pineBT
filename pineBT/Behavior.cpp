#include "Behavior.h"

using namespace pineBT;

Behavior::Result Behavior::run()
{
	if (result != Result::RUNNING) onEnter();
	result = update();
	if (result != Result::RUNNING) onExit(result);
	return result;
}

bool Behavior::isCompleted() const
{
	return result == Result::SUCCESS || result == Result::FAILURE;
}

void Behavior::abort()
{
	if (result == Result::RUNNING) 
	{
		onExit(Result::ABORTED);
		result = Result::ABORTED;
	}
}

std::string pineBT::name(Behavior::Result result)
{
	switch (result)
	{
	case Behavior::Result::RUNNING: return "RUNNING";
	case Behavior::Result::FAILURE: return "FAILURE";
	case Behavior::Result::SUCCESS: return "SUCCESS";
	case Behavior::Result::ABORTED: return "ABORTED";
	case Behavior::Result::INVALID: return "INVALID";
	}
}