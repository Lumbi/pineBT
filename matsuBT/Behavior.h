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

		~Behavior() {};

		virtual void onEnter() {};

		virtual Result run() = 0;

		virtual void onExit(Result) {};
	};
}
