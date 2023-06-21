#pragma once

#include "Behavior.h"

#include <vector>

namespace pineBT
{
	class Composite : public Behavior
	{
	public:
		void addChild(Behavior*);

		virtual void abort() override;

	protected:
		using Behaviors = std::vector<Behavior*>;
		Behaviors children;
	};
}
