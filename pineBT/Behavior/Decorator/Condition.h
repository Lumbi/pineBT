#pragma once

#include "Decorator.h"

namespace pineBT
{
	class Condition : public Decorator
	{
	protected:
		virtual bool check() = 0;

		virtual Result update() override;
	};
}