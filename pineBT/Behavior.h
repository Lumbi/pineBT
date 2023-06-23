#pragma once

#include <vector>
#include <memory>
#include <string>
#include <format>

#include "Behavior/Schema/BehaviorSchema.h"

namespace pineBT
{
	class Behavior
	{
	public:
		using ID = uint32_t;

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

		Result getResult() const { return result; }

		bool isCompleted() const;

		virtual void abort();

		virtual std::string toString() const { return "Behavior"; }

	public:
		ID id = 0;

	protected:
		virtual void onEnter() {}

		virtual Result update() = 0;

		virtual void onExit(Result) {}

	private:
		Result result = Result::INVALID;
	};
	
	std::string name(Behavior::Result);
}
