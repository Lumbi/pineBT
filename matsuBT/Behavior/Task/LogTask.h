#pragma once

#include "Task.h"

namespace matsuBT
{
	class LogTask : public Task
	{
	public:
		LogTask(const char* message);

		virtual Result update() override;

	private:
		const char* message;
	};
}