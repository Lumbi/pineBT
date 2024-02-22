#pragma once

#include "Behavior.h"

#include <vector>

namespace pineBT
{
	class Composite : public Behavior
	{
	public:
		static BehaviorSchema schema;

	public:
		using Behaviors = std::vector<std::unique_ptr<Behavior>>;

	public:
		virtual void addChild(std::unique_ptr<Behavior>) override;

		const Behaviors& getChildren() const { return children; };

		virtual void abort() override;

	protected:
		Behaviors children;
	};
}
