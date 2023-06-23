#pragma once

#include "Behavior.h"

#include <memory>
#include <iterator>
#include <queue>

namespace pineBT
{
	class BehaviorTree
	{
		friend struct BehaviorTreeBuilder;

	public:
		BehaviorTree(class LinearAllocator& allocator)
			: root(nullptr),
			  allocator(allocator)
		{};

		BehaviorTree(BehaviorTree&) = delete;
		BehaviorTree& operator=(BehaviorTree) = delete;

		static struct BehaviorTreeBuilder build(class LinearAllocator&);

		Behavior* getRoot() const;

		void setRoot(Behavior*);

		void run();

		void print();

		struct BehaviorTreeQuery query();

	private:
		Behavior* root;
		class LinearAllocator& allocator;

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
