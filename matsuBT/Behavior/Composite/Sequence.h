#pragma once

#include "Composite.h"

namespace matsuBT
{
	class Sequence : public Composite
	{
	public:
		virtual void onEnter() override;

		virtual Result update() override;

	private:
		Behaviors::iterator currentChild;
	};
}