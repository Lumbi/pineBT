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

		~Behavior() {}

		Result run();

	protected:
		virtual void onEnter() {}

		virtual Result update() = 0;

		virtual void onExit(Result) {}

	private:
		Result result = Result::INVALID;
	};
}
