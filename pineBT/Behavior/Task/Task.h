#pragma once

#include "Behavior.h"

namespace pineBT
{
	class Task : public Behavior
	{
	public:
		std::string toString() const override { return std::format("Task [{}]", name(getResult())); }
	};
}