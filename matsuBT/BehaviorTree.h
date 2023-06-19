#pragma once

#include <memory>

#include "Behavior.h"

namespace matsuBT
{
	class BehaviorTree
	{
	public:
		BehaviorTree();

		void setRoot(std::unique_ptr<Behavior>);

	private:
		std::unique_ptr<Behavior> root;
	};
}
