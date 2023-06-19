#pragma once

#include "Composite.h"

namespace matsuBT
{
	class Selector : public Composite
	{
	public:
		virtual void onEnter() override;

		virtual Result update() override;

	protected:
		Behaviors::iterator currentChild;
	};
}