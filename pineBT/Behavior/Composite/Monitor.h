#pragma once

#include "Parallel.h"

namespace pineBT
{
	class Monitor : public Parallel
	{
	public:
		Monitor() : Parallel(Parallel::Policy::ALL, Parallel::Policy::ANY) {}

		std::string toString() const override { return std::format("Monitor [{}]", name(getResult())); }

		void addCondition(std::unique_ptr<Behavior>);

		void addAction(std::unique_ptr<Behavior>);

	private:
		void addChild(std::unique_ptr<Behavior> child) override { Parallel::addChild(std::move(child)); }
	};
}
