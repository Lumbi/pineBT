#pragma once

#include "Behavior.h"

#include <memory>

namespace matsuBT
{
	class Composite : public Behavior
	{
	public:
		void addChild(std::unique_ptr<Behavior>);

	protected:
		using Behaviors = std::vector<std::unique_ptr<Behavior>>;
		Behaviors children;
	};
}
