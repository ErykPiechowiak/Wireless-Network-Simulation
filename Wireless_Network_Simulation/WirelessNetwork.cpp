#include "WirelessNetwork.h"
#include "NewPacketEvent.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include "EndOfTransmissionEvent.h"
#include "ChannelListeningEvent.h"
#include "Logger.h"
#include "AckCheckEvent.h"
#include "Event.h"
#include "Generator.h"


WirelessNetwork::WirelessNetwork(const double L, const int K, Logger *logger,Event::EventList *event_list,std::vector<int> *seeds) : kL_(L), kK_(K), logger_(logger),event_list_(event_list),seeds_(seeds)
{
	logger->Debug("Created a Wireless Network");
	init();
}


WirelessNetwork::~WirelessNetwork()
{
	delete(channel_);
	while (!receivers_->empty())
	{
		Receiver *receiver = receivers_->back();
		receivers_->pop_back();
		delete(receiver);
	}
	delete(receivers_);
	while (!transmitters_->empty())
	{
		Transmitter *transmitter = transmitters_->back();
		transmitters_->pop_back();
		delete(transmitter);
	}
	delete(transmitters_);
	while (!generators_->empty())
	{
		Generator *generator = generators_->back();
		generators_->pop_back();
		delete(generator);

	}
	delete(generators_);


}

bool WirelessNetwork::getInitialPhaseFlag()
{
	return initial_phase_;
}

int WirelessNetwork::getPacketsDelivered()
{
	return packets_delivered_;
}

void WirelessNetwork::turnOffInitialPhase()
{
	initial_phase_ = false;
}

void WirelessNetwork::incrementPacketsCounter(int transmitter_index)
{
	if (!initial_phase_)
	{
		Transmitter *transmitter = transmitters_->at(transmitter_index);
		transmitter->updateRetransmissionsCounter();
		transmitter->incrementPacketCounter();
		transmitter->calculatePacketErrorRate();
		packets_delivered_++;
	}

}

double WirelessNetwork::calculatePacketErrorRate()
{
	if (!initial_phase_)
	{
		double packet_error_rate_sum = 0;
		Transmitter *transmitter;
		for (int i = 0; i < kK_; ++i)
		{
			transmitter = transmitters_->at(i);
			packet_error_rate_sum += static_cast<double>(transmitter->getPacketErrorRate());
		}
		packet_error_rate_ = packet_error_rate_sum / kK_;
	}
	return packet_error_rate_;
}

double WirelessNetwork::calculateAvgNumberOfRetransmissions()
{
	int sum_of_retransmissions = 0;
	for (int i = 0; i < kK_; ++i)
	{
		sum_of_retransmissions += transmitters_->at(i)->getRetransmissionsCounter();
	}
	return static_cast<double>(sum_of_retransmissions) / static_cast<double>(packets_delivered_);
}

double WirelessNetwork::calculateThroughput(size_t time)
{
	double time_in_seconds = static_cast<double>(time) / 10000;
	return static_cast<double>(packets_delivered_) / static_cast<double>(time_in_seconds);
}

double WirelessNetwork::calculateAvgDelay()
{
	double avarge_delay = 0;
	delay_time_sum_ /= 10;
	avarge_delay = static_cast<double>(delay_time_sum_) / static_cast<double>(packets_delivered_);
	delay_time_sum_ *= 10;
	return avarge_delay;
}

double WirelessNetwork::calculateAvgWaitingTime()
{
	double avg_waiting_time = 0;
	waiting_time_sum_ /= 10;
	avg_waiting_time = static_cast<double>(waiting_time_sum_) / (static_cast<double>(packets_delivered_) + static_cast<double>(transmissions_failed_counter_));
	return avg_waiting_time;
}

double WirelessNetwork::getMaximalPacketErrorRate()
{
	double maximal_packet_error_rate = 0;
	Transmitter *transmitter;
	for (int i = 0; i < kK_; ++i)
	{
		maximal_packet_error_rate = (maximal_packet_error_rate < transmitters_->at(i)->getPacketErrorRate()) ?
			transmitters_->at(i)->getPacketErrorRate() : maximal_packet_error_rate;
	}
	return maximal_packet_error_rate;
}

void WirelessNetwork::init()
{
	
	transmitters_ = new std::vector<Transmitter*>;
	receivers_ = new std::vector<Receiver*>;
	generators_ = new std::vector<Generator*>;
	initGenerators();
	createTransmitters();
	createReceivers();
	channel_ = new Channel(logger_);
}

void WirelessNetwork::initGenerators()
{
	ter_generator_ = new Generator(get_seed());
	generators_->push_back(ter_generator_);

}

void WirelessNetwork::addDelayTime(int delay)
{
	delay_time_sum_ += delay;
}

Transmitter * WirelessNetwork::getTransmitter(int transmitter_index)
{
	return transmitters_->at(transmitter_index);
}

int WirelessNetwork::checkChannelListeningTime(int transmitter_index)
{
	return transmitters_->at(transmitter_index)->get_ch_time();
}

bool WirelessNetwork::checkChannel()
{
	return channel_->isEmpty();
}

bool WirelessNetwork::checkTransmissionFlag(int transmitter_index)
{

	return transmitters_->at(transmitter_index)->getTransmissionFlag();
}

bool WirelessNetwork::checkAckEventFlag(int transmitter_index)
{
	return transmitters_->at(transmitter_index)->getAckEventFlag();
}

bool WirelessNetwork::checkChannelListeningFlag(int transmitter_index)
{
	return transmitters_->at(transmitter_index)->getChannelListeningFlag();
}

bool WirelessNetwork::checkRetransmissionEventFlag(int transmitter_index)
{
	return transmitters_->at(transmitter_index)->getRetransmissionFlag();
}

bool WirelessNetwork::checkDiscardPacketEventFlag(int transmitter_index)
{
	return transmitters_->at(transmitter_index)->getDiscardPacketFlag();
}

bool WirelessNetwork::checkDelayEventFlag(int transmitter_index)
{
	return transmitters_->at(transmitter_index)->getDelayFlag();
}

bool WirelessNetwork::checkChannelCheckingEventFlag(int transmitter_index)
{
	return transmitters_->at(transmitter_index)->getChannelCheckingEvent();
}



void WirelessNetwork::increaseTransmitterChTime(int transmitter_index)
{
	Transmitter *current_transmitter = transmitters_->at(transmitter_index);
	current_transmitter->increaseChTime();
}


int WirelessNetwork::getTransmitterBt(int transmitter_index)
{
	return transmitters_->at(transmitter_index)->get_bt();
}

bool WirelessNetwork::getReceiverAckFlag(int receiver_index)
{
	return receivers_->at(receiver_index)->get_ack();
}

void WirelessNetwork::setDelayEventFlag(int transmitter_index)
{
	transmitters_->at(transmitter_index)->setDelayFlag(true);
}

void WirelessNetwork::setDiscardPacketEventFlag(int transmitter_index)
{
	transmitters_->at(transmitter_index)->setDiscardPacketFlag(true);
}

void WirelessNetwork::setRetransmissionEventFlag(int transmitter_index)
{
	transmitters_->at(transmitter_index)->setRetransmissionFlag(true);
}

void WirelessNetwork::setSuccessfulTransmissionFlag(int transmitter_index)
{
	transmitters_->at(transmitter_index)->setSuccessfulTransmission(true);
}

void WirelessNetwork::restartTransmitterChTime(int transmitter_index)
{
	transmitters_->at(transmitter_index)->set_ch_time(0);
}

void WirelessNetwork::resetChannelListeningFlag(int transmitter_index)
{
	transmitters_->at(transmitter_index)->setChannelListeningFlag(false);
}

void WirelessNetwork::resetTransmitterAckEventFlag(int transmitter_index)
{
	transmitters_->at(transmitter_index)->setAckEventFlag(false);
}

bool WirelessNetwork::isBufferEmpty(int transmitter_index)
{
	if (transmitters_->at(transmitter_index)->get_buffer_size() == 0)
	{
		return true;
	}
	return false;
}

void WirelessNetwork::channelChecking(int transmitter_index,size_t clock, bool channel_state)
{
	Transmitter *transmitter = transmitters_->at(transmitter_index);
	if (channel_state)
	{
		if (checkChannelListeningTime(transmitter_index) == 30 && !checkDelayEventFlag(transmitter_index))
		{
			transmitter->set_bt(0);
			transmitter->setTransmissionFlag(true);
			transmitter->setChannelListeningFlag(true);
			transmitter->setChannelCheckingEvent(false);
		}
		else if (checkChannelListeningTime(transmitter_index) == 30)
		{
			transmitter->setTransmissionFlag(true);
			transmitter->setChannelListeningFlag(true);
			transmitter->setChannelCheckingEvent(false);
		}
		else
		{
			increaseTransmitterChTime(transmitter_index);
			double time_info_ = static_cast<double>(checkChannelListeningTime(transmitter_index)) / 10; //used to display time in [ms]
			logger_->Debug("Transmitter " + std::to_string(transmitter_index) + ": ch_time update: " + std::to_string(time_info_));
			transmitter->setChannelCheckingEvent(true);
			size_t execution_time = clock + static_cast<size_t>(5);
			ChannelListeningEvent *ev = new ChannelListeningEvent(execution_time, getTransmitter(transmitter_index), logger_);
			event_list_->push(ev);
		}
	}
	else
	{
		restartTransmitterChTime(transmitter_index);
		transmitter->set_bt(-1);
		transmitter->setDelayFlag(true);
		double time_info_ = static_cast<double>(checkChannelListeningTime(transmitter_index)) / 10; //used to display time in [ms]
		logger_->Debug("Transmitter " + std::to_string(transmitter_index) + ": ch_time update: " + std::to_string(time_info_));
		size_t execution_time = clock + static_cast<size_t>(5);
		ChannelListeningEvent *ev = new ChannelListeningEvent(execution_time, getTransmitter(transmitter_index), logger_);
		event_list_->push(ev);
	}
}

void WirelessNetwork::packetTransmissionEvent(int transmitter_index,size_t clock)
{
	Transmitter* transmitter = transmitters_->at(transmitter_index);
	channel_->add_packet(transmitter->get_packet());
	if (!transmitter->getTransmissionStartedFlag() && !initial_phase_)
	{
		transmitter->setTransmissionStartedFlag(true);
		Packet *packet = transmitter->get_packet();
		size_t added_to_the_buffer_time = packet->getAddedToTheBufferTime();
		transmitter->setPacketTransmissionStartedTime(clock);
	}
	if (channel_->get_size() > 1)
		channel_->failedTransmission();
	size_t execution_time = static_cast<size_t>(transmitter->get_packet()->get_ctp()) + clock;
	EndOfTransmissionEvent *ev = new EndOfTransmissionEvent(channel_, execution_time,
		transmitter->get_packet()->get_id(), receivers_, logger_);
	event_list_->push(ev);
	transmitter->set_bt(-1);
	transmitter->set_ch_time(0);	
	transmitter->setDelayFlag(false);
	transmitter->setChannelListeningFlag(false);
	execution_time = static_cast<size_t>(transmitter->get_packet()->get_ctp()) + static_cast<size_t>(kCtiz_) + clock;
	AckCheckEvent *ev2 = new AckCheckEvent(transmitter,execution_time, logger_);
	event_list_->push(ev2);
	transmitter->setTransmissionFlag(false);
}

void WirelessNetwork::retransmissionEvent(int transmitter_index, size_t clock)
{
	Transmitter *transmitter = transmitters_->at(transmitter_index);
	transmitter->setRetransmissionFlag(false);
	logger_->Debug("Transmitter " + std::to_string(transmitter->get_id()) + " RetransmissionEvent started, because conditions were met");

	if (transmitter->get_cw() < transmitter->get_cwmax())
	{
		transmitter->incrementRetransmissionsCounter();
		transmitter->generateNewTer();
		transmitter->cwUpdate();
		transmitter->setDelayFlag(true);
		transmitter->set_ch_time(30);			//changing channel listening time to 30 (3ms) will trigger Delay Event
		size_t execution_time = clock + static_cast<size_t>(5);
		ChannelListeningEvent *ev = new ChannelListeningEvent(execution_time, transmitter, logger_);
		event_list_->push(ev);
	}
	//cw > cw_max - discard packet
	else
	{
		double time_info_ = static_cast<double>(clock) / 10; //used to display time in [ms]
		logger_->Debug("Simulation time: " + std::to_string(time_info_) + " [ms]");
		logger_->Debug("Receiver " + std::to_string(transmitter->get_id()) + ": transmission failed");
		if (!initial_phase_)
		{
			transmissions_failed_counter_++;
			transmitter->incrementFailedTransmissionCounter();
			transmitter->incrementPacketCounter();
			transmitter->calculatePacketErrorRate();
		}
		transmitter->setDiscardPacketFlag(true); //delete packet from buffer
	}
}

void WirelessNetwork::delayEvent(int transmitter_index, size_t clock, bool channel_state)
{
	Transmitter *transmitter = transmitters_->at(transmitter_index);
	if (transmitter->get_bt() == -1) // bt = -1 -> delay wasn't generated yet 
	{
		logger_->Debug("Transmitter " + std::to_string(transmitter->get_id()) + ": delayEvent started, because conditions were met");
		transmitter->generateNewBt();

		logger_->Debug("Transmitter " + std::to_string(transmitter->get_id()) + ": new bt value = " + std::to_string(transmitter->get_bt()));
	}
	
	if (channel_state)
	{
		if (transmitter->get_bt() == 0)
		{
			transmitter->setTransmissionFlag(true);
			transmitter->setDelayFlag(false);
		}
		else
		{ 
			transmitter->decrementBT();
			transmitter->setChannelListeningFlag(false);		
			logger_->Debug("Transmitter " + std::to_string(transmitter->get_id()) + ": updated bt value = " + std::to_string(transmitter->get_bt()));
			size_t execution_time = clock + static_cast<size_t>(5);
			ChannelListeningEvent *ev = new ChannelListeningEvent(execution_time, transmitter, logger_);
			event_list_->push(ev);
		}
	}
	else
	{
		transmitter->setChannelListeningFlag(false);
		size_t execution_time = clock + static_cast<size_t>(5);
		ChannelListeningEvent *ev = new ChannelListeningEvent(execution_time, transmitter, logger_);
		event_list_->push(ev);
	}
}

void WirelessNetwork::discardPacketEvent(int transmitter_index,size_t execution_time,size_t simulation_time)
{
	Transmitter *transmitter = transmitters_->at(transmitter_index);
	Packet *packet = transmitter->get_packet();
	size_t added_to_the_buffer_time = packet->getAddedToTheBufferTime();
	//Collects data needed to calculate avg waiting time 
	waiting_time_sum_ += transmitter->getPacketTransmissionStartedTime() - added_to_the_buffer_time;
	//Collects data needed to calculate avg delay 
	if (transmitter->getSuccessfulTransmissionFlag() && !initial_phase_) //make sure that packet was sent after initial phase
	{		
		delay_time_sum_ += simulation_time - added_to_the_buffer_time;
	}
	logger_->Debug("Transmitter " + std::to_string(transmitter->get_id()) + " DiscardPacketEvent started, because conditions were met");
	transmitter->deletePacket();
	transmitter->resetTransmitter();	//sets all transmitter values to default
	logger_->Debug("Transmitter " + std::to_string(transmitter->get_id()) + ": deleted pacekt. Number of packets in buffer : " + std::to_string(transmitter->get_buffer_size()));
}


void WirelessNetwork::createTransmitters()
{
	for (int i = 0; i < kK_; ++i)
	{
		Generator *exp_generator = new Generator(get_seed());
		Generator *transmission_time_generator_ = new Generator(get_seed());
		Generator *delay_generator = new Generator(get_seed());
		generators_->push_back(exp_generator);
		generators_->push_back(transmission_time_generator_);
		generators_->push_back(delay_generator);
		Transmitter *new_transmitter = new Transmitter(i, kL_,logger_,ter_generator_,delay_generator);
		transmitters_->push_back(new_transmitter);
		//creating first NewPacketEvent for each transmitter
		Event *ev = new NewPacketEvent(i,0, new_transmitter,channel_, exp_generator,transmission_time_generator_,
			ter_generator_, kL_,logger_,event_list_);
		//adding NewPacketEvent to the event calendar
		event_list_->push(ev);
	}
}

void WirelessNetwork::createReceivers()
{
	for (int i = 0; i < kK_; ++i)
	{
		Receiver *new_receiver = new Receiver(i,logger_);
		receivers_->push_back(new_receiver);
	}
}


int WirelessNetwork::get_seed()
{
	int seed = seeds_->back();
	seeds_->pop_back();
	return seed;
}

