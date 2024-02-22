#pragma once

#include "Behavior.h"
#include "Blackboard/Blackboard.h"

#include <memory>
#include <iterator>
#include <queue>

namespace pineBT
{
	class BehaviorTree
	{
	public:
		BehaviorTree(const Blackboard& blackboard)
			: root(nullptr),
			  blackboard(blackboard)
		{};

		BehaviorTree(BehaviorTree&) = delete;
		BehaviorTree& operator=(BehaviorTree) = delete;

		~BehaviorTree() = default;

		static struct BehaviorTreeBuilder build(const Blackboard&);

		Behavior* getRoot() const;

		void setRoot(std::unique_ptr<Behavior>);

		const Blackboard& getBlackboard() const { return blackboard; };

		void run();

		void print();

		struct BehaviorTreeQuery query();

	private:
		std::unique_ptr<Behavior> root;
		const Blackboard& blackboard; // TODO: Make blackboard an std::shared_ptr

		// Iterator support

	public:
		struct iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using value_type = Behavior;
			using pointer = value_type*;
			using reference = value_type&;

			iterator(pointer start) : pointers({ start }) {}
			iterator() = default;

			reference operator*() const;
			pointer operator->();

			iterator& operator++();
			iterator operator++(int);

			friend bool operator== (const iterator& lhs, const iterator& rhs);
			friend bool operator!= (const iterator& lhs, const iterator& rhs);

		private:
			std::queue<pointer> pointers;
		};

		iterator begin() { return iterator(root.get()); };
		iterator end() { return iterator(); };
		iterator begin() const { return iterator(root.get()); };
		iterator end() const { return iterator(); };
	};
}
