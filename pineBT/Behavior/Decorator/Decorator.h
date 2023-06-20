#pragma once

#include "Behavior.h"

#include <memory>

namespace pineBT
{
	class Decorator : public Behavior
	{
	public:
		void setChild(std::unique_ptr<Behavior>);

	protected:
		std::unique_ptr<Behavior> child;
	};
}