#ifndef WIRELESSNETWORK_H_
#define WIRELESSNETWORK_H_

#include <vector>
//#include <random>
#include "Channel.h"
#include "Receiver.h"
#include "Transmitter.h"
#include "Logger.h"
#include "Event.h"
#include "Generator.h"
// class WirelessNetwork creates vector of transmitters, vector od receivers and transmission channel

class WirelessNetwork
{
public:
	WirelessNetwork(const double L, const int K, Logger *logger,Event::EventList *event_list,std::vector<int> *seeds);
	~WirelessNetwork();
	Transmitter* getTransmitter(int transmitter_index);
	int checkChannelListeningTime(int transmitter_index);
	bool checkChannel();	//checks if channel is empty 
	bool checkTransmissionFlag(int transmitter_index);
	bool checkAckEventFlag(int transmitter_index);
	bool checkChannelListeningFlag(int transmitter_index);
	bool checkRetransmissionEventFlag(int transmitter_index);
	bool checkDiscardPacketEventFlag(int transmitter_index);
	bool checkDelayEventFlag(int transmitter_index);
	bool checkChannelCheckingEventFlag(int transmitter_index);
	void increaseTransmitterChTime(int transmitter_index);
	int getTransmitterBt(int transmitter_index);
	bool getReceiverAckFlag(int receiver_index);
	void setDelayEventFlag(int transmitter_index);
	void setDiscardPacketEventFlag(int transmitter_index);
	void setRetransmissionEventFlag(int transmitter_index);
	void setSuccessfulTransmissionFlag(int transmitter_index);
	void restartTransmitterChTime(int transmitter_index);
	void resetChannelListeningFlag(int transmitter_index);
	void resetTransmitterAckEventFlag(int transmitter_index);
	bool isBufferEmpty(int transmitter_index);
	//EVENTS
	void channelChecking(int transmitter_index,size_t clock, bool channel_state);
	void packetTransmissionEvent(int transmitter_index, size_t clock);
	void retransmissionEvent(int transmitter_index,size_t clock);
	void delayEvent(int transmitter_index,size_t clock, bool channel_state);
	void discardPacketEvent(int transmitter_index, size_t execution_time,size_t simulation_time); //this event is triggered if transmission was sucesfull
																			// or if retransmission cw > cw_max (retransmission failed)
	//statistics
	bool getInitialPhaseFlag();
	int getPacketsDelivered();
	void turnOffInitialPhase();
	void incrementPacketsCounter(int transmitter_index);
	double calculatePacketErrorRate();
	double calculateAvgNumberOfRetransmissions();
	double calculateThroughput(size_t time);
	double calculateAvgDelay();
	double calculateAvgWaitingTime();
	double getMaximalPacketErrorRate();
private:
	void init();
	void initGenerators();
	void addDelayTime(int delay);
	void createTransmitters();
	void createReceivers();
	int get_seed();
	const double kL_;		//intensivity of exponential distribution
	const int kK_;			// Number of transmitters and receivers 
	const int kCtiz_ = 10;	// Time of ACK transmission / 10*10^-4[s] 

	Logger *logger_=nullptr;
	std::vector<Transmitter*> *transmitters_=nullptr;
	std::vector<Receiver*> *receivers_=nullptr;
	std::vector<int> *seeds_ = nullptr;
	Channel* channel_=nullptr;
	//generators
	Generator *ter_generator_ = nullptr;
	std::vector<Generator*> *generators_ = nullptr; //contains pointer to every used generator, it's used to delete them at the end of the simulation

	Event::EventList *event_list_ = nullptr;	//event calendar
	//statistics
	bool initial_phase_ = true;
	double packet_error_rate_ = 0;
	int packets_delivered_ = 0; //packets delivered after initial phase
	int transmissions_failed_counter_ = 0;
	size_t delay_time_sum_ = 0; //collects data needed to calculate avg delay 
	size_t waiting_time_sum_ = 0; //collects data needed to calculate avg waiting time
	
	
};

#endif // !WIRELESSNETWORK_H_



