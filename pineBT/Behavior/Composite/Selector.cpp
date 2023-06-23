#include "Selector.h"

#include <assert.h>

using namespace pineBT;

BehaviorSchema Selector::schema = BehaviorSchema::inherit(
    "Selector",
    Composite::schema,
    BehaviorSchema::Options().build()
        .boolean("live")
    .end()
);

// Standard Selector

void Selector::onEnter()
{
    currentChild = children.begin();
}

Behavior::Result Selector::update()
{
    assert(currentChild != children.end());

    while (true)
    {
        const Result result = (*currentChild)->run();

        if (result != Result::FAILURE)
            return result;

        if (++currentChild == children.end())
            return Result::FAILURE;
    }

    return Result::INVALID;
}

// Live Selector

void LiveSelector::onEnter()
{
    Selector::onEnter();
}

Behavior::Result LiveSelector::update()
{
    assert(!children.empty());

    Behaviors::iterator previousChild = currentChild;
    currentChild = children.begin();
    Result result = Selector::update();
    // Abort previously running child
    if (previousChild != children.end() && previousChild != currentChild) 
    {
        (*previousChild)->abort();
    }

    return result;
}
