#pragma once

#include "Behavior/Decorator/Condition.h"

namespace pineBT
{
	class MockCondition : public Condition
	{
	public:
		using Condition::Condition;

		void setForceCheck(bool forceCheck) { this->forceCheck = forceCheck; }

	private:
		bool forceCheck = false;

		virtual bool check() override;
	};
}
