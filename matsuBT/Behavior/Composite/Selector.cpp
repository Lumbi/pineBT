#include "Selector.h"

using namespace matsuBT;

Behavior::Result Selector::run()
{
    for (auto&& child : children)
    {
        const Result result = child->run();
        if (result == Result::FAILURE)
            continue; 
        else 
            return result;
    }
    return Result::FAILURE;
}
