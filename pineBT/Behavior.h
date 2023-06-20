#pragma once

#include <vector>
#include <memory>

namespace pineBT
{
	class Behavior
	{
	public:
		enum class Result
		{
			RUNNING,
			FAILURE,
			SUCCESS,
			ABORTED,
			INVALID
		};

	public:
		Behavior() = default;

		~Behavior() {}

		Result run();

		void abort();

	protected:
		virtual void onEnter() {}

		virtual Result update() = 0;

		virtual void onExit(Result) {}

	private:
		Result result = Result::INVALID;
	};
}
