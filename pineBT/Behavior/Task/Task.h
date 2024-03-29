#pragma once

#include "Behavior.h"

namespace pineBT
{
	class Task : public Behavior
	{
	public:
		static BehaviorSchema schema;

	public:
		virtual void addChild(std::unique_ptr<Behavior>) override {};

		std::string toString() const override { 
			return std::format("Task #{} [{}]", id, name(getResult())); 
		}
	};
}
