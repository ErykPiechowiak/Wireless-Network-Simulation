#include "Receiver.h"
#include <iostream>
Receiver::Receiver(int id,Logger *logger):id_(id),logger_(logger)
{
	logger_->Debug("Created a new Receiver id: " + std::to_string(id_));
}

Receiver::~Receiver()
{
	
}

void Receiver::receivePacket(Packet* packet)
{
	packet_ = packet;
	ack_ = packet_->getIsCorrectFlag();
	logger_->Debug("Receiver " + std::to_string(id_) + ": ack flag update: " + std::to_string(ack_));
}

bool Receiver::get_ack()
{
	return ack_;
}

