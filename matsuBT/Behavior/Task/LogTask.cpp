#include "LogTask.h"

using namespace matsuBT;

LogTask::LogTask(const char* message)
	: message(message)
{
}

Behavior::Result LogTask::update()
{
	printf("%s\n", message);
	return Result::SUCCESS;
}
