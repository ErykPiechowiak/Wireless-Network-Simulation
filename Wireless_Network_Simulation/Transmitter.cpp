#include "Transmitter.h"
#include <random>
#include <iostream>
#include <random>
#include <algorithm>
#include "Packet.h"
#include <String>
Transmitter::Transmitter(int id, double L,Logger *logger,Generator *ter_generator,Generator *delay_generator)
{

	ter_generator_ = ter_generator;
	delay_generator_ = delay_generator;
	logger_ = logger;
	id_ = id;
	buffer_ = new std::queue<Packet*>;
	cw_ = kCwMin_;
	std::string info_message = "Created a new Transmitter id: "+std::to_string(id_);
	logger_->Debug(info_message);
}

Transmitter::~Transmitter()
{
	while(!buffer_->empty())
	{
		Packet *packet = buffer_->front();
		buffer_->pop();
		delete(packet);

	}
}
std::queue<Packet*>* Transmitter::get_buffer()
{
	return buffer_;
}
Packet * Transmitter::get_packet()
{
	return buffer_->front();
}
int Transmitter::get_cw()
{
	return cw_;
}
int Transmitter::get_id()
{
	return id_;
}
int Transmitter::get_bt()
{
	return bt_;
}
int Transmitter::get_cwmax()
{
	return kCwMax_;
}
int Transmitter::get_ch_time()
{
	return ch_time_;
}

int Transmitter::get_buffer_size()
{
	return buffer_->size();
}
bool Transmitter::getTransmissionFlag()
{
	return transmission_flag_;
}
bool Transmitter::getAckEventFlag()
{
	return ack_event_flag_;
}
bool Transmitter::getChannelListeningFlag()
{
	return channel_listening_flag_;
}
bool Transmitter::getRetransmissionFlag()
{
	return retransmission_flag_;
}
bool Transmitter::getDiscardPacketFlag()
{
	return discard_packet_;
}
bool Transmitter::getDelayFlag()
{
	return delay_flag_;
}
bool Transmitter::getChannelCheckingEvent()
{
	return channel_checking_event;
}
bool Transmitter::getSuccessfulTransmissionFlag()
{
	return successful_transmission_;
}
bool Transmitter::getTransmissionStartedFlag()
{
	return transmission_started_;
}
void Transmitter::setTransmissionStartedFlag(bool flag)
{
	transmission_started_ = flag;
}

void Transmitter::set_bt(int bt)
{
	
	bt_ = bt;
}
void Transmitter::set_ch_time(int ch_time)
{
	ch_time_ = ch_time;
}
void Transmitter::setDelayFlag(bool flag)
{
	delay_flag_ = flag;
}
void Transmitter::setRetransmissionFlag(bool retransmit)
{
	retransmission_flag_ = retransmit;
}
void Transmitter::setDiscardPacketFlag(bool discard_packet)
{
	discard_packet_ = discard_packet;
}
void Transmitter::setAckEventFlag(bool ack)
{
	ack_event_flag_ = ack;
}
void Transmitter::setTransmissionFlag(bool flag)
{
	transmission_flag_ = flag;
}
void Transmitter::setChannelListeningFlag(bool flag)
{
	channel_listening_flag_ = flag;
}
void Transmitter::setChannelCheckingEvent(bool flag)
{
	channel_checking_event = flag;
}
void Transmitter::setSuccessfulTransmission(bool flag)
{
	successful_transmission_ = flag;
}
void Transmitter::cwUpdate()
{
	cw_ = std::min(2 * (cw_ + 1) - 1, kCwMax_);
}

void Transmitter::decrementBT()
{
	bt_--;
}
void Transmitter::increaseChTime()
{
	ch_time_ += 5;
}
void Transmitter::addPacket(Packet * packet)
{
	buffer_->push(packet);
}
void Transmitter::deletePacket()
{
	buffer_->pop();
}
void Transmitter::generateNewBt()
{
	int bt = static_cast<int>(delay_generator_->Rand(1, cw_));
	bt_ = bt;
}
void Transmitter::generateNewTer()
{
	bool ter = ter_generator_->RndZeroOne(kTerProb_);
	buffer_->front()->set_ter(ter);
}

void Transmitter::resetTransmitter()
{
	cw_ = kCwMin_;
	retransmission_flag_ = false;
	discard_packet_ = false;
	ch_time_ = 0;
	bt_ = -1;
	channel_listening_flag_ = true;
	transmission_flag_ = false;
	ack_event_flag_ = false;
	delay_flag_ = false;
	channel_checking_event = true;
	packet_error_ = false;
	successful_transmission_ = false;
	temp_retransmissions_counter_ = 0;
	transmission_started_ = false;

}

void Transmitter::updateRetransmissionsCounter()
{
	retransmissions_counter_ += temp_retransmissions_counter_;
}

void Transmitter::incrementRetransmissionsCounter()
{
	temp_retransmissions_counter_++;
}

void Transmitter::incrementPacketCounter()
{
	packets_counter_++;
}

void Transmitter::incrementFailedTransmissionCounter()
{
	failed_transmission_counter_++;
}

double Transmitter::getPacketErrorRate()
{
	return packet_error_rate_;
}

int Transmitter::getRetransmissionsCounter()
{
	return retransmissions_counter_;
}

void Transmitter::calculatePacketErrorRate()
{
	packet_error_rate_ = static_cast<double>(failed_transmission_counter_) / static_cast<double>(packets_counter_);
}

void Transmitter::setErrorPacketFlag()
{
	packet_error_ = true;
}


void Transmitter::setPacketTransmissionStartedTime(size_t time)
{
	packet_transmission_started_time_ = time;
}

bool Transmitter::getErrorPacketFlag()
{
	return packet_error_;
}


size_t Transmitter::getPacketTransmissionStartedTime()
{
	return packet_transmission_started_time_;
}

