#pragma once

#include "Parallel.h"

namespace pineBT
{
	class Monitor : public Parallel
	{
	public:
		Monitor() : Parallel(Parallel::Policy::ALL, Parallel::Policy::ANY) {}

		std::string toString() const override { return std::format("Monitor [{}]", name(getResult())); }

		void addCondition(Behavior*);

		void addAction(Behavior*);

	private:
		void addChild(Behavior* child) override { Parallel::addChild(child); }
	};
}
