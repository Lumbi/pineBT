#pragma once

#include "Composite.h"

namespace pineBT
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