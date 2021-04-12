#include "Event.h"

Event::Event(size_t execution_time) : execution_time_(execution_time)
{
}

Event::Event()
{
}

Event::~Event()
{

}

void Event::setExecutionTime(int execution_time)
{
	execution_time_ = execution_time;
}

size_t Event::get_time()
{
	return execution_time_;
}

