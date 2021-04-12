#include "NewPacketEvent.h"
#include <iostream>
#include "ChannelListeningEvent.h"


NewPacketEvent::NewPacketEvent(int packet_id,int clock, Transmitter *transmitter,Channel *channel, Generator *exp_generator,
	Generator *transmission_time_generator, Generator *ter_generator, const double L,Logger *logger,Event::EventList *event_list):
	packet_id_(packet_id),clock_(clock),transmitter_(transmitter),channel_(channel),exp_generator_(exp_generator),
	transmission_time_generator_(transmission_time_generator),ter_generator_(ter_generator),
	kL_(L),logger_(logger),event_list_(event_list)
{
	transmission_time_ = setTransmissionTime();
	setExecutionTime(set_cgp(kL_)+clock_);
	logger_->Debug("Transmitter " + std::to_string(transmitter_->get_id()) + ": Created a new NewPacketEvent");
	double time_info = static_cast<double>(execution_time_) / 10; //used to display time in [ms]
	double time_info2 = static_cast<double>(transmission_time_) / 10;
	logger_->Debug("NewPacketEvent execution time: " + std::to_string(time_info) + " [ms] New packet transmission time " +
		std::to_string(time_info2) + " [ms]");
}

NewPacketEvent::~NewPacketEvent()
{

}

void NewPacketEvent::execute()
{
	Packet *new_packet = new Packet(packet_id_, transmission_time_);
	new_packet->set_ter(generateTer());
	new_packet->setAddedToBufferTime(execution_time_);
	transmitter_->addPacket(new_packet);
	logger_->Debug("Transmitter " + std::to_string(transmitter_->get_id()) + ": added a new packet to the buffer. Number of packets in buffer: " + std::to_string(transmitter_->get_buffer_size()));
	Event *ev = new NewPacketEvent(packet_id_,execution_time_,transmitter_,channel_,exp_generator_,
		transmission_time_generator_,ter_generator_,kL_,logger_,event_list_);
	//adding NewPacketEvent to the event calendar
	event_list_->push(ev);

}
int NewPacketEvent::set_cgp(const double L)
{
	int cgp = static_cast<int>(exp_generator_->RndExp(L));
	return cgp;
}


int NewPacketEvent::setTransmissionTime()
{
	int transmission_time = static_cast<int>(transmission_time_generator_->Rand(1, 10));
	transmission_time *= 10;
	return transmission_time;
}

bool NewPacketEvent::generateTer()
{

	return ter_generator_->RndZeroOne(kTerProb_);

}


