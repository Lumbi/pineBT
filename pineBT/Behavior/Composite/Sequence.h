#pragma once

#include "Composite.h"

namespace pineBT
{
	class Sequence : public Composite
	{
	public:
		virtual void onEnter() override;

		virtual Result update() override;

		std::string toString() const override { return std::format("Sequence [{}]", name(getResult())); }

	private:
		Behaviors::iterator currentChild;
	};
}