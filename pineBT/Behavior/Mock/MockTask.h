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
		MockTask(Result forceResult = Result::SUCCESS) : forceResult(forceResult) {}

		virtual void configure(const Option&) override;

		virtual Result update() override;

		void setForceResult(Result forceResult);

	private:
		Result forceResult;
	};
}
