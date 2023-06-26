#pragma once

#include "Composite.h"

namespace pineBT
{
	class Parallel : public Composite
	{
	public:
		static BehaviorSchema schema;

	public:
		enum class Policy
		{
			ANY,
			ALL
		};

		Parallel(
			Policy successPolicy = Policy::ALL,
			Policy failurePolicy = Policy::ANY
		) : 
			successPolicy(successPolicy), 
			failurePolicy(failurePolicy)
		{}

		virtual void configure(const Option&) override;

		std::string toString() const override {
			return std::format("Parallel #{} [{}]", id, name(getResult()));
		}

		void setSuccessPolicy(Policy successPolicy) { this->successPolicy = successPolicy; }

		void setFailurePolicy(Policy failurePolicy) { this->failurePolicy = failurePolicy; }

		virtual void addChild(Behavior* child) { Composite::addChild(child); }

	protected:
		Result update() override;

		void onExit(Result) override;

	private:
		Policy successPolicy;
		Policy failurePolicy;
	};
}
