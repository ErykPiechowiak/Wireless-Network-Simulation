#ifndef CHANNELLISTENINGEVENT_H_
#define CHANNELLISTENINGEVENT_H_

#include "Event.h"
#include "Channel.h"
#include "Transmitter.h"
#include "Logger.h"
//The task of this class is to manage ch_time and bt fields of transmitters
class ChannelListeningEvent :
	public Event
{
public:
	ChannelListeningEvent(size_t execution_time,Transmitter *transmitter, Logger *logger);
	~ChannelListeningEvent();
	virtual void execute();
private:
	Transmitter *transmitter_;
	Logger *logger_;
	double time_info_;

};
#endif // !CHANNELLISTENINGEVENT_H_



