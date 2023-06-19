#pragma once

#include "Behavior.h"

#include <memory>

namespace matsuBT
{
	class Composite : public Behavior
	{
	protected:
		using Behaviors = std::vector<std::unique_ptr<Behavior>>;
		Behaviors children;
	};
}
