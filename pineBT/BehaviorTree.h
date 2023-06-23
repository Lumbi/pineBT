#pragma once

#include "Behavior.h"
#include "Builder/BehaviorTreeBuilder.h"

#include <memory>
#include <iterator>
#include <queue>

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


		// Iterator support

	public:
		struct iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = Behavior;
			using pointer = value_type*;
			using reference = value_type&;

			iterator(pointer start) : pointers({ start }) {}
			iterator() {}

			reference operator*() const;
			pointer operator->();

			iterator& operator++();
			iterator operator++(int);

			friend bool operator== (const iterator& lhs, const iterator& rhs);
			friend bool operator!= (const iterator& lhs, const iterator& rhs);

		private:
			std::queue<pointer> pointers;
		};

		iterator begin() { return iterator(root); };
		iterator end() { return iterator(); };
		iterator begin() const { return iterator(root); };
		iterator end() const { return iterator(); };
	};
}
