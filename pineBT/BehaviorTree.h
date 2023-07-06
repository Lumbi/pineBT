#pragma once

#include "Behavior.h"
#include "Blackboard/Blackboard.h"
#include "Memory/LinearAllocator.h"

#include <memory>
#include <iterator>
#include <queue>

namespace pineBT
{
	class BehaviorTree
	{
		using Allocator = LinearAllocator;

	public:
		BehaviorTree(Allocator& allocator, const Blackboard& blackboard)
			: root(nullptr),
			  allocator(allocator),
			  blackboard(blackboard)
		{};

		BehaviorTree(BehaviorTree&) = delete;
		BehaviorTree& operator=(BehaviorTree) = delete;

		static struct BehaviorTreeBuilder build(Allocator&, const Blackboard&);

		Behavior* getRoot() const;

		void setRoot(Behavior*);

		Allocator& getAllocator() { return allocator; };

		const Blackboard& getBlackboard() const { return blackboard; };

		void run();

		void print();

		struct BehaviorTreeQuery query();

	private:
		Behavior* root;
		Allocator& allocator;
		const Blackboard& blackboard;

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
