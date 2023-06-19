#pragma once

#include <memory>

namespace matsuBT
{
	class SelectorBuilder
	{
	public:
		SelectorBuilder(class BehaviorTreeBuilder& builder) : builder(builder) {}

		SelectorBuilder& task(std::unique_ptr<class Task> task);

		class BehaviorTreeBuilder& end();

	private:
		class BehaviorTreeBuilder& builder;
	};
}
