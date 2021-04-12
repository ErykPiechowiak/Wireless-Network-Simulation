#ifndef ACKCHECKEVENT_H_
#define ACKCHECKEVENT_H_

#include "Event.h"
#include "Transmitter.h"
#include "Receiver.h"
#include "Logger.h"
//Class AckCheckEvent checks if the ack flag was set
class AckCheckEvent :
	public Event
{
public:
	AckCheckEvent(Transmitter *transmitter, int execution_time,Logger *logger);
	~AckCheckEvent();
	virtual void execute();
private:
	Transmitter *transmitter_;
	Logger *logger_;

};
#endif // !ACKCHECKEVENT_H_



