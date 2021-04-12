#include "Channel.h"
#include <iostream>
Channel::Channel(Logger *logger):logger_(logger)
{
	logger->Debug("Created a new Channel");
}

Channel::~Channel()
{
}

void Channel::add_packet(Packet * packet)
{
	ch_packets_.push_back(packet);
}

bool Channel::isEmpty()
{
	if (ch_packets_.empty())
	{
		return true;
	}
	return false;
}

int Channel::get_size()
{
	return ch_packets_.size();
}

void Channel::failedTransmission()
{
	logger_->Debug("Collision!");
	for (int i = 0; i < ch_packets_.size(); ++i)
	{
		ch_packets_[i]->failedTransmission();
	}
}

Packet * Channel::get_packet(int packet_id)
{
	Packet *packet = nullptr;
	std::vector<Packet*>::iterator nth;
	for (int i = 0; i < ch_packets_.size(); ++i)
	{
		if (ch_packets_[i]->get_id() == packet_id)
		{
			packet = ch_packets_[i];
			nth = ch_packets_.begin() + i;
			break;
		}
	}
	ch_packets_.erase(nth);
	return packet;
	
	
	
}
