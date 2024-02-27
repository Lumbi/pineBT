#pragma once

#include "Condition.h"
#include "Blackboard/Blackboard.h"

namespace pineBT
{
	class BlackboardCondition : public Condition
	{
	public:
		static BehaviorSchema schema;

		enum class Predicate
		{
			EXIST,
			EQUAL,
			LESS_THAN,
			GREATER_THAN
		};

	public:
		BlackboardCondition(
			const Blackboard& blackboard,
			Blackboard::Key key = 0,
			Predicate predicate = Predicate::EXIST,
			float target = 0.f
		)
			: Condition(),
			  blackboard(blackboard),
			  key(key),
			  predicate(predicate),
			  target(target)
		{}

		void set(Blackboard::Key, Predicate, float target);

		virtual void configure(const Option&) override;

		virtual bool check() override;

	private:
		const Blackboard& blackboard;
		Blackboard::Key key;
		Predicate predicate;
		float target;
	};
}
