#ifndef EVENT_H_
#define EVENT_H_
#include <vector>
#include <functional>
#include <queue>
class Event
{
public: 

	typedef std::priority_queue<Event*, std::vector<Event*>, std::function<bool(Event*, Event*)>>EventList; //event calendar
	Event(size_t execution_time);
	Event();
	~Event();
	virtual void execute() = 0;
	void setExecutionTime(int execution_time);
	size_t get_time();
protected:
	size_t execution_time_;
};
#endif // !EVENT_H_



