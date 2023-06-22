#pragma once

#include "Decorator.h"

namespace pineBT
{
	class Condition : public Decorator
	{
	public:
		enum class Mode
		{
			INSTANT,
			CONTINUOUS
		};

		Condition(Mode mode = Mode::INSTANT) : mode(mode) {}

		std::string toString() const override { return std::format("Condition [{}]", name(getResult())); }

	protected:
		virtual bool check() = 0;

		virtual Result update() override;

	private:
		Mode mode;
	};
}