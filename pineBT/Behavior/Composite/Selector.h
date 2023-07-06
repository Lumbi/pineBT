#pragma once

#include "Composite.h"

namespace pineBT
{
	class Selector : public Composite
	{
	public:
		static BehaviorSchema schema;

	public:
		virtual void configure(const Option&) override;

		void setLive(bool);

		virtual void onEnter() override;

		virtual Result update() override;

		std::string toString() const override { 
			return std::format(
				"{}Selector #{} [{}]",
				live ? "Live " : "",
				id,
				name(getResult())
			); 
		}

	protected:
		Behaviors::iterator currentChild;

	private:
		bool live = false;
	};
}
