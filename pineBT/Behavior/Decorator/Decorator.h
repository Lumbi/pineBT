#pragma once

#include "Behavior.h"

namespace pineBT
{
	class Decorator : public Behavior
	{
	public:
		static BehaviorSchema schema;

	public:
		virtual void addChild(std::unique_ptr<Behavior>);

		void setChild(std::unique_ptr<Behavior>);

		Behavior* getChild() const;

		std::string toString() const override { return std::format("Decorator [{}]", name(getResult())); }

		virtual void abort() override;

	protected:
		std::unique_ptr<Behavior> child;
	};
}
