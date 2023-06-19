#pragma once

#include "Composite.h"

namespace matsuBT
{
	class Selector : public Composite
	{
		virtual Result run() override;
	};
}