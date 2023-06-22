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

		enum class Logic : bool
		{
			DEFAULT = true,
			NEGATE = false
		};

		Condition(Mode mode = Mode::INSTANT, Logic logic = Logic::DEFAULT)
			: mode(mode), logic(logic) {}

		std::string toString() const override {
			return std::format("{}Condition [{}]", logic == Logic::NEGATE ? "NOT " : "", name(getResult()));
		}

	protected:
		virtual bool check() = 0;

		virtual Result update() override;

	private:
		Mode mode;
		Logic logic;
	};
}