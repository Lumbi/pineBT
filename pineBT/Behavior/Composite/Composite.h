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
		using Behaviors = std::vector<Behavior*>;

	public:
		void addChild(Behavior*);

		const Behaviors& getChildren() const { return children; };

		virtual void abort() override;

	protected:
		Behaviors children;
	};
}
