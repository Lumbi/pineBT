#pragma once

#include "Composite.h"

namespace matsuBT
{
	class Sequence : public Composite
	{
		virtual Result run() override;
	};
}