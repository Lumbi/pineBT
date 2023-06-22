#pragma once

#include "Behavior/Decorator/Condition.h"

namespace pineBT
{
	class MockCondition : public Condition
	{
	public:
		MockCondition(bool forceCheck, Condition::Mode mode = Condition::Mode::INSTANT)
			: Condition(mode), forceCheck(forceCheck) 
		{}

		void setForceCheck(bool forceCheck) { this->forceCheck = forceCheck; }

	private:
		bool forceCheck;

		virtual bool check() override;
	};
}
