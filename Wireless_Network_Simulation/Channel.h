#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <vector>
#include "Packet.h"
#include "Logger.h"
//This class represents transmission channel 
class Channel
{
public:
	Channel(Logger *logger);
	~Channel();
	void add_packet(Packet* packet);//adds packet to the ch_packets_
	bool isEmpty();					//returns true if channel is empty
	int get_size();
	void failedTransmission();		//sets every packet is_correct value to false
	Packet* get_packet(int packet_id);//returns packet of specific id and delete it from ch_packets_ 

private:
	std::vector<Packet*> ch_packets_; //packets being currently transmitted 
	Logger *logger_ = nullptr;

};
#endif // !CHANNEL_H



