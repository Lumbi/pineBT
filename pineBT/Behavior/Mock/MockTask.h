#pragma once

#include "Behavior/Task/Task.h"

#include <string>

namespace pineBT
{
	class MockTask : public Task
	{
	public:
		static BehaviorSchema schema;

	public:
		MockTask(
			const std::string& message = "",
			Result forceResult = Result::SUCCESS
		)
			: message(message), forceResult(forceResult)
		{};

		virtual Result update() override;

		void setMessage(const std::string& message);

		void setForceResult(Result forceResult);

	private:
		std::string message;
		Result forceResult;
	};
}
