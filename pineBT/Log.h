#pragma once

#include <stdio.h>

namespace pineBT
{
	namespace log
	{
#ifdef PINE_BT_ENABLE_LOG
		template<typename ... Args>
		inline void print(const char* format, const Args & ... args) noexcept
		{
			printf("[pineBT] ");
			printf(format, args ...);
		}
#else
		inline void print(const char* format, ...) noexcept {}
#endif
	}
}
