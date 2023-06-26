#include "Selector.h"

#include <assert.h>

using namespace pineBT;

static const char* OPT_LIVE = "live";

BehaviorSchema Selector::schema = BehaviorSchema::inherit(
    "Selector",
    Composite::schema,
    BehaviorSchema::Options().build()
        .boolean(OPT_LIVE)
    .end()
);

void Selector::configure(const Option& option)
{
    if (option.key == OPT_LIVE)
    {
        assert(option.value.tag == Option::Value::Type::BOOLEAN);
        setLive(option.value.asBoolean);
    }
}

void Selector::setLive(bool live)
{
    this->live = live;
}

void Selector::onEnter()
{
    currentChild = children.begin();
}

Behavior::Result Selector::update()
{
    Behaviors::iterator previousChild;

    if (live)
    {
        // Reset to first child
        assert(!children.empty());
        previousChild = currentChild;
        currentChild = children.begin();
    }

    // Normal selector logic
    assert(currentChild != children.end());
    Result result;

    while (true)
    {
        const Result childResult = (*currentChild)->run();

        if (childResult != Result::FAILURE)
        {
            result = childResult;
            break;
        }

        if (++currentChild == children.end())
        {
            result = Result::FAILURE;
            break;
        }
    }

    result = Result::INVALID;
 
    if (live) {
        // Abort previously running child
        if (previousChild != children.end() && previousChild != currentChild)
            (*previousChild)->abort();
    }
    
    return result;
}
