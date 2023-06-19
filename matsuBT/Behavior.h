#pragma once

#include <vector>
#include <memory>

namespace matsuBT
{
	class Behavior
	{
	public:
		enum class Result
		{
			RUNNING,
			FAILURE,
			SUCCESS,
			INVALID
		};

	public:
		Behavior() = default;

		virtual Result run() = 0;
	};
}
