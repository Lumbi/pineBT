#pragma once

#include <memory>

namespace pineBT
{
	class LinearAllocator
	{
	public:
		LinearAllocator(std::size_t size) 
			: buffer(new std::byte[size]),
			  offset(0)
		{};

		~LinearAllocator()
		{
			delete[] buffer;
		}

		LinearAllocator(LinearAllocator&) = delete;
		LinearAllocator& operator=(LinearAllocator&) = delete;

		template<typename Data, typename... Args>
		Data* allocate(Args&&... args)
		{
			Data* data = new ((void*)(buffer + offset)) Data(std::forward<Args>(args)...);
			offset += sizeof(Data);
			return data;
		}

	private:
		std::byte* const buffer;
		std::size_t offset;
	};
}