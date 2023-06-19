#pragma once

#include "Behavior.h"

#include <memory>

namespace matsuBT
{
	class Composite : public Behavior
	{
	protected:
		std::vector<std::unique_ptr<Behavior>> children;
	};
}
