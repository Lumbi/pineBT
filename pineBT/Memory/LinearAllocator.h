#pragma once

#include <memory>

namespace pineBT
{
	class LinearAllocator
	{
	public:
		LinearAllocator(std::size_t size) 
			: buffer(new std::byte[size]),
			  offset(0),
			  size(size)
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
			const std::size_t chunk = sizeof(Data);
			if (offset + chunk > size) { return nullptr; }
			Data* data = new ((void*)(buffer + offset)) Data(std::forward<Args>(args)...);
			offset += chunk;
			return data;
		}

	private:
		std::byte* const buffer;
		std::size_t offset;
		std::size_t size;
	};
}