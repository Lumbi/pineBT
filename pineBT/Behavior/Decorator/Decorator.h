#pragma once

#include "Behavior.h"

namespace pineBT
{
	class Decorator : public Behavior
	{
	public:
		void setChild(Behavior*);

	protected:
		Behavior* child = nullptr;
	};
}