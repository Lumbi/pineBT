#pragma once

#include "Behavior.h"

#include <vector>

namespace pineBT
{
	class Composite : public Behavior
	{
	public:
		void addChild(Behavior*);

	protected:
		using Behaviors = std::vector<Behavior*>;
		Behaviors children;
	};
}
