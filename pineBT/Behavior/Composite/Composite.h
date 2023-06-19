#pragma once

#include "Behavior.h"

#include <memory>

namespace pineBT
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
