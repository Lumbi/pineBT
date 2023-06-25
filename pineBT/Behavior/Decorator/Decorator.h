#pragma once

#include "Behavior.h"

namespace pineBT
{
	class Decorator : public Behavior
	{
	public:
		static BehaviorSchema schema;

	public:
		virtual void addChild(Behavior*);

		void setChild(Behavior*);

		Behavior* getChild() const;

		std::string toString() const override { return std::format("Decorator [{}]", name(getResult())); }

		virtual void abort() override;

	protected:
		Behavior* child = nullptr;
	};
}