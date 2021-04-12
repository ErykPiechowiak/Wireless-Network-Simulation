#ifndef ENDOFTRANSMISSIONEVENT_H_
#define ENDOFTRANSMISSIONEVENT_H_

#include <vector>
#include "Event.h"
#include "Channel.h"
#include "Receiver.h"
#include "Logger.h"
class EndOfTransmissionEvent :
	public Event
{
public:
	EndOfTransmissionEvent(Channel* channel, int execution_time,int packet_id,std::vector<Receiver*> *receivers,Logger *logger);
	~EndOfTransmissionEvent();
	virtual void execute();//this method checks if there was a collision in the channel and passes packet to the receiver
private:
	std::vector<Receiver*> *receivers_;
	Channel* channel_;
	int packet_id_;
	Logger *logger_;
	
};
#endif // !ENDOFTRANSMISSIONEVENT_H_


