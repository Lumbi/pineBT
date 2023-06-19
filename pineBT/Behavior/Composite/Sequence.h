#pragma once

#include "Composite.h"

namespace pineBT
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