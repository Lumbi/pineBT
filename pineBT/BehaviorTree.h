#pragma once

#include "Behavior.h"
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
		BehaviorTree(Allocator& allocator)
			: root(nullptr),
			  allocator(allocator)
		{};

		BehaviorTree(BehaviorTree&) = delete;
		BehaviorTree& operator=(BehaviorTree) = delete;

		static struct BehaviorTreeBuilder build(Allocator&);

		Behavior* getRoot() const;

		void setRoot(Behavior*);

		Allocator& getAllocator() { return allocator; };

		void run();

		void print();

		struct BehaviorTreeQuery query();

	private:
		Behavior* root;
		Allocator& allocator;

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
