#pragma once

#include "BehaviorTree.h"
#include "Behavior/Composite/Composite.h"
#include "Behavior/Decorator/Decorator.h"
#include "Behavior/Task/Task.h"

#include <stack>
#include <stdio.h>

namespace pineBT
{
	class BehaviorTreePrinter
	{
	public:
		BehaviorTreePrinter(BehaviorTree& tree) : tree(tree) {};

		void print();

	private:
		BehaviorTree& tree;
	};
}