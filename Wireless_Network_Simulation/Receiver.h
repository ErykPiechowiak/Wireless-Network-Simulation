#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "Packet.h"
#include "Logger.h"
// class Receiver is responsible for sending ack flag
class Receiver
{
public:
	Receiver(int id,Logger *logger);
	~Receiver();
	void receivePacket(Packet* packet);
	bool get_ack();
private:
	int id_;
	Packet* packet_;
	bool ack_ = false; //if packet was transmitted correctly, ack_ will be true 
	Logger *logger_ = nullptr;
};
#endif // !RECEIVER_H_



