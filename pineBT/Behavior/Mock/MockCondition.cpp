#include "MockCondition.h"

using namespace pineBT;

bool MockCondition::check()
{
	return forceCheck;
}
