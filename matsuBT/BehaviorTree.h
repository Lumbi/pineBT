#pragma once

#include "Behavior.h"
#include "Builder/BehaviorTreeBuilder.h"

#include <memory>

namespace matsuBT
{
	class BehaviorTree
	{
	public:
		BehaviorTree();

		static BehaviorTreeBuilder build();

		Behavior* getRoot() const;

		void setRoot(std::unique_ptr<Behavior>);

		void run();

	private:
		std::unique_ptr<Behavior> root;
	};
}
