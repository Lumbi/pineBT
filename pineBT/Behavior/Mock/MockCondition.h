#pragma once

#include "Behavior/Decorator/Condition.h"

namespace pineBT
{
	class MockCondition : public Condition
	{
	public:
		static BehaviorSchema schema;

	public:
		using Condition::Condition;

		virtual void configure(const Option&) override;

		void setForceCheck(bool value) { this->forceCheck = value; }

	private:
		bool forceCheck = false;

		virtual bool check() override;
	};
}
