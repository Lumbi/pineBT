#pragma once

#include "Decorator.h"

namespace pineBT
{
	class Condition : public Decorator
	{
	public:
		std::string toString() const override { return std::format("Condition [{}]", name(getResult())); }

	protected:
		virtual bool check() = 0;

		virtual Result update() override;
	};
}