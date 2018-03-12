#include "counter.h"

Counter::Counter(int initialValue, bool infinite, int limit, int extension)
{
    this->value     = initialValue;
    this->infinite  = infinite;
    this->limit     = limit;
    this->extension = extension;
}

int Counter::get_limit()
{
    return limit;
}

int Counter::get_value()
{
    return value;
}

bool Counter::is_infinite()
{
    return infinite;
}

void Counter::inc()
{
    value++;
}

bool Counter::limit_reached()
{
    return !infinite && value >= limit;
}

void Counter::extend_limit()
{
    limit += extension;
}
