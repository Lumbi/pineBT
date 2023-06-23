#pragma once

#include "Behavior.h"

#include <memory>
#include <iterator>
#include <queue>

namespace pineBT
{
	class BehaviorTree
	{
	public:
		BehaviorTree(std::size_t maxMemorySize = 2048);

		~BehaviorTree();

		BehaviorTree(BehaviorTree&) = delete;
		BehaviorTree& operator=(BehaviorTree) = delete;

		static struct BehaviorTreeBuilder build();

		Behavior* getRoot() const;

		void setRoot(Behavior*);

		void run();

		void print();

		struct BehaviorTreeQuery query();

		template<typename Data, typename... Args>
		Data* allocate(Args&&... args)
		{
			Data* data = new ((void*)(buffer + offset)) Data;
			offset += sizeof(Data);
			return data;
		}

	private:
		Behavior* root;
		std::byte* buffer;
		std::size_t offset;


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
