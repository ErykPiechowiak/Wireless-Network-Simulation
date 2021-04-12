#include "Packet.h"
#include <random>
#include <iostream>
Packet::Packet(int id,int ctp):id_(id),ctp_(ctp)
{
}

Packet::~Packet()
{
}

void Packet::failedTransmission()
{
	is_correct_ = false;
}

int Packet::get_id()
{
	return id_;
}

int Packet::get_ctp()
{
	return ctp_;
}

bool Packet::getIsCorrectFlag()
{
	return is_correct_;
}

void Packet::set_ter(bool ter)
{
	is_correct_ = ter;
}

void Packet::setAddedToBufferTime(size_t time)
{
	added_to_buffer_time_ = time;
}

size_t Packet::getAddedToTheBufferTime()
{
	return added_to_buffer_time_;
}



