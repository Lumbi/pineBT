#include "Selector.h"

using namespace matsuBT;

void Selector::onEnter()
{
    currentChild = children.begin();
}

Behavior::Result Selector::run()
{
#ifdef DEBUG
    if (currentChild == children.end()) { return Result::INVALID; }
#endif

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
