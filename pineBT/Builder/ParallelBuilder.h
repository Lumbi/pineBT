#pragma once

#include "Behavior/Composite/Parallel.h"

namespace pineBT
{
	struct ParallelBuilder
	{
		ParallelBuilder(
			class BehaviorTreeBuilder& builder,
			class Parallel* parallel
		) :
			builder(builder),
			parallel(parallel)
		{}

	protected:
		BehaviorTreeBuilder& builder;
		Parallel* parallel;
	};

	struct ParallelFailurePolicyBuilder : public ParallelBuilder
	{
		using ParallelBuilder::ParallelBuilder;

		BehaviorTreeBuilder& failWhenAny();
		BehaviorTreeBuilder& failWhenAll();
	};

	struct ParallelSuccessPolicyBuilder : public ParallelBuilder
	{
		using ParallelBuilder::ParallelBuilder;

		ParallelFailurePolicyBuilder succeedWhenAny();
		ParallelFailurePolicyBuilder succeedWhenAll();
	};
}