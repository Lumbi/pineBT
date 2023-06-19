#include "Selector.h"

#include <assert.h>

using namespace pineBT;

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
