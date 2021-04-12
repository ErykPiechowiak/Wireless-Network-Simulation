#include "EndOfTransmissionEvent.h"

EndOfTransmissionEvent::EndOfTransmissionEvent(Channel * channel, int execution_time,int packet_id,std::vector<Receiver*> *receivers,Logger *logger):
	channel_(channel),Event(execution_time),packet_id_(packet_id),receivers_(receivers),logger_(logger)
{
	logger_->Debug("Transmitter " + std::to_string(packet_id_) + ": Created a new EndOfTransmissionEvent");
	double time_info = static_cast<double>(execution_time_)/10;
	logger_->Debug("EndOfTransmissionEvent execution time: " + std::to_string(time_info) + " [ms]");
}

EndOfTransmissionEvent::~EndOfTransmissionEvent()
{
	
}

void EndOfTransmissionEvent::execute()
{
	Packet* packet = channel_->get_packet(packet_id_);
	receivers_->at(packet_id_)->receivePacket(packet);
	logger_->Debug("Receiver: " + std::to_string(packet_id_) + " Packet received");
}
