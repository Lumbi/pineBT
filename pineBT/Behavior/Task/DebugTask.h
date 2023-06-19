#pragma once

#include "Task.h"

namespace pineBT
{
	class DebugTask : public Task
	{
	public:
		DebugTask(const char* message, Result forceResult = Result::SUCCESS)
			: message(message), forceResult(forceResult)
		{};

		virtual Result update() override;

	private:
		const char* message;
		Result forceResult;
	};
}
