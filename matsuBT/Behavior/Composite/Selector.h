#pragma once

#include "Composite.h"

namespace matsuBT
{
	class Selector : public Composite
	{
		virtual void onEnter() override;

		virtual Result run() override;

	protected:
		Behaviors::iterator currentChild;
	};
}