#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_

#include <queue>
//#include <random>
#include "Packet.h"
#include "Logger.h"
#include "Generator.h"
//This class represents a transmitter which creates packets and transmits them
class Transmitter
{
public:
	Transmitter(int id, double L, Logger *logger,Generator *ter_generator_,Generator *delay_generator);//L is the intensity of an exponential distribution
	~Transmitter();
	std::queue<Packet*>* get_buffer();
	Packet* get_packet();			//gets first packet from the buffer
	int get_cw();
	int get_id();
	int get_bt();
	int get_cwmax();
	int get_ch_time();
	int get_buffer_size();
	bool getTransmissionFlag();
	bool getAckEventFlag();
	bool getChannelListeningFlag();
	bool getRetransmissionFlag();
	bool getDiscardPacketFlag();
	bool getDelayFlag();
	bool getChannelCheckingEvent();
	bool getSuccessfulTransmissionFlag();
	bool getTransmissionStartedFlag();
	void setTransmissionStartedFlag(bool flag);
	void set_bt(int bt);
	void set_ch_time(int ch_time);
	void setDelayFlag(bool flag);
	void setRetransmissionFlag(bool retransmit);
	void setDiscardPacketFlag(bool discard_packet);
	void setAckEventFlag(bool ack);
	void setTransmissionFlag(bool flag);
	void setChannelListeningFlag(bool flag);
	void setChannelCheckingEvent(bool flag);
	void setSuccessfulTransmission(bool flag);
	void cwUpdate();
	void decrementBT();
	void increaseChTime();
	void addPacket(Packet* packet);
	void deletePacket();
	void generateNewBt();
	void generateNewTer();				//generates new ter value of packet after retransmission event
	void resetTransmitter();			//sets all fields to the default values
	//statistics
	void updateRetransmissionsCounter();
	void incrementRetransmissionsCounter();
	void incrementPacketCounter();
	void incrementFailedTransmissionCounter();
	double getPacketErrorRate();
	int getRetransmissionsCounter();
	bool getErrorPacketFlag();
	size_t getPacketTransmissionStartedTime();
	void calculatePacketErrorRate();
	void setErrorPacketFlag();
	void setPacketTransmissionStartedTime(size_t time);
	
private:
	const int kCwMax_ = 255;
	const int kCwMin_ = 15;
	const double kTerProb_ = 0.8;
	bool retransmission_flag_ = false; //if true - retransmission event will be triggered 
	bool discard_packet_ = false;	  //if true - discard packet event will be triggered
	int ch_time_ = 0;					//time spent on a channel listening 
	int id_;							
	int bt_ = -1;						
	int cw_;							//contention window
	bool channel_checking_event = true;
	bool ack_event_flag_ = false;
	bool transmission_flag_ = true;
	bool channel_listening_flag_ = true;
	bool delay_flag_ = false;
	bool successful_transmission_ = false;
	bool transmission_started_ = false; //used to calculate avg waiting time
	std::queue<Packet*> *buffer_;		
	Logger *logger_ = nullptr;
	Generator *ter_generator_ = nullptr; //used to generate ter
	Generator *delay_generator_ = nullptr;

	//statistics
	bool packet_error_ = false;
	double packet_error_rate_ = 0;
	int packets_counter_ = 0;
	int errors_counter_ = 0;
	int failed_transmission_counter_ = 0;
	int retransmissions_counter_ = 0; //Updated after successful transmission
	int temp_retransmissions_counter_ = 0; //Only counts retransmissions of one packet
	size_t packet_transmission_started_time_ = 0;


};

#endif // !TRANSMITTER_H_


