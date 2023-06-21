#pragma once

#include "Composite.h"

namespace pineBT
{
	class Selector : public Composite
	{
	public:
		virtual void onEnter() override;

		virtual Result update() override;

		std::string toString() const override { return std::format("Selector [{}]", name(getResult())); }

	protected:
		Behaviors::iterator currentChild;
	};

	class LiveSelector : public Selector
	{
	public:
		void onEnter() override;

		Result update() override;

		std::string toString() const override { return std::format("Live Selector [{}]", name(getResult())); }
	};
}