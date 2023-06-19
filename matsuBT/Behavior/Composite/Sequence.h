#pragma once

#include "Composite.h"

namespace matsuBT
{
	class Sequence : public Composite
	{
		virtual void onEnter() override;

		virtual Result run() override;

	private:
		Behaviors::iterator currentChild;
	};
}