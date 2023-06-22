#include "ParallelBuilder.h"

using namespace pineBT;

ParallelFailurePolicyBuilder ParallelSuccessPolicyBuilder::succeedWhenAny()
{
    parallel->setSuccessPolicy(Parallel::Policy::ANY);
    return ParallelFailurePolicyBuilder(builder, parallel);
}

ParallelFailurePolicyBuilder ParallelSuccessPolicyBuilder::succeedWhenAll()
{
    parallel->setSuccessPolicy(Parallel::Policy::ALL);
    return ParallelFailurePolicyBuilder(builder, parallel);
}

BehaviorTreeBuilder& ParallelFailurePolicyBuilder::failWhenAny()
{
    parallel->setFailurePolicy(Parallel::Policy::ANY);
    return builder;
}

BehaviorTreeBuilder& ParallelFailurePolicyBuilder::failWhenAll()
{
    parallel->setFailurePolicy(Parallel::Policy::ALL);
    return builder;
}
