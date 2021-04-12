#ifndef NEWPACKETEVENT_H_
#define NEWPACKETEVENT_H_
#include "Event.h"
#include <queue>
#include "packet.h"
#include "Transmitter.h"
#include "Channel.h"
#include "Logger.h"
#include "Generator.h"

class NewPacketEvent :
	public Event
{
public:
	NewPacketEvent(int packet_id, int clock, Transmitter *transmitter, Channel *channel, Generator *exp_generator,
		Generator *transmission_time_generator, Generator *ter_generator, const double L, Logger *logger, Event::EventList *event_list);
	~NewPacketEvent();
	virtual void execute();//adds packet to the buffer of the transmitter and creates another NewPacketEvent
private:
	int set_cgp(double L);
	int setTransmissionTime();
	bool generateTer();
	const double kL_;
	const double kTerProb_ = 0.8; //probability of setting TER flag to 'true'
	Transmitter *transmitter_;
	int transmission_time_; 
	int packet_id_;
	Channel *channel_;
	Generator *exp_generator_ = nullptr;
	Generator *transmission_time_generator_ = nullptr;
	Generator *ter_generator_ = nullptr;
	Logger *logger_;
	int clock_;
	Event::EventList *event_list_;
};
#endif // !NEWPACKETEVENT_H_



