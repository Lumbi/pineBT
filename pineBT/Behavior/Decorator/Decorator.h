#pragma once

#include "Behavior.h"

namespace pineBT
{
	class Decorator : public Behavior
	{
	public:
		void setChild(Behavior*);

		std::string toString() const override { return std::format("Decorator [{}]", name(getResult())); }

		virtual void abort() override;

	protected:
		Behavior* child = nullptr;
	};
}