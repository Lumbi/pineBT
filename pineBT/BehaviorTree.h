#pragma once

#include "Behavior.h"
#include "Builder/BehaviorTreeBuilder.h"

#include <memory>

namespace pineBT
{
	class BehaviorTree
	{
	public:
		BehaviorTree();

		static BehaviorTreeBuilder build();

		Behavior* getRoot() const;

		void setRoot(Behavior*);

		void run();

		void print();

		template<typename BehaviorType, typename... Args>
		BehaviorType* allocate(Args&&... args)
		{
			auto behavior = std::make_unique<BehaviorType>(std::forward<Args>(args)...);
			BehaviorType* pointer = behavior.get();
			behaviors.push_back(std::move(behavior));
			return pointer;
		}

	private:
		Behavior* root;
		std::vector<std::unique_ptr<Behavior>> behaviors;
	};
}
