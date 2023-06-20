#pragma once

#include "Behavior/Task/Task.h"

namespace pineBT
{
	class MockTask : public Task
	{
	public:
		MockTask(const char* message, Result forceResult = Result::SUCCESS)
			: message(message), forceResult(forceResult)
		{};

		virtual Result update() override;

		void setForceResult(Result forceResult);

	private:
		const char* message;
		Result forceResult;
	};
}
