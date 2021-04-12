#include "Simulator.h"
#include "Event.h"
#include "ChannelListeningEvent.h"
#include <iostream>
#include <limits>
#include <cstdio>
#include <fstream>


char question()
{
	std::string choice;
	std::cout << "Would you like to run simulation in step-by-step mode? (y/n) ";
	std::cin >> choice;
	if (choice.size() > 1)
	{
		return 'x';
	}
	if (choice[0] == 'y' || choice[0] == 'Y')
	{
		return 'y';
	}
	else if (choice[0] == 'n' || choice[0] == 'N')
	{
		return 'n';
	}	
	return 'x';

}



Simulator::Simulator(const int K, double L, Logger *logger, int simulation_nr, int simulation_type,Logger::Level level) :kK_(K),kL_(L),logger_(logger),
simulation_nr_(simulation_nr), simulation_type_(simulation_type),level_(level)
{
	init();
	loadSeeds();
}


void Simulator::savePacketErrorRate(double packet_error_rate, size_t time,bool initial_phase)
{
	if (!initial_phase)
	{
		std::fstream file("./statistics/packet_error_rate_sim_nr_"+std::to_string(simulation_nr_)+".txt", std::ios_base::app);
		std::string line;
		line = std::to_string(packet_error_rate);
		if (file.is_open())
		{
			file << line << "\n";
		}
		file.close();

		std::fstream file2("./statistics/packet_error_rate_time_sim_nr_" + std::to_string(simulation_nr_) + ".txt", std::ios_base::app);
		double temp = static_cast<double>(time) / 10;
		line = std::to_string(temp);
		if (file2.is_open())
		{
			file2 << line << "\n";
		}
		file.close();
	}

}


void Simulator::saveResults(WirelessNetwork *wireless_network)
{
	std::fstream file("./statistics/results_sim_nr_" + std::to_string(simulation_nr_) + ".txt", std::ios_base::app);

	if (file.is_open())
	{
		file << "RESULTS \n";
		file << "Avarge packet error rate: " + std::to_string(wireless_network->calculatePacketErrorRate()) + "\n";
		file << "Maximal packet error rate: " + std::to_string(wireless_network->getMaximalPacketErrorRate()) + "\n";
		file << "Avarge number of retransmissions: " + std::to_string(wireless_network->calculateAvgNumberOfRetransmissions())+"\n";
		file << "Throughput: " + std::to_string(wireless_network->calculateThroughput(clock_)) + " [packets/s] \n";
		file << "Avarge packet delay: " + std::to_string(wireless_network->calculateAvgDelay()) + " [ms] \n";
		file << "Avarge waiting time: " + std::to_string(wireless_network->calculateAvgWaitingTime()) + " [ms] \n";

	}
	file.close();
	
}


void Simulator::saveAvgNumberOfRetransmissions(double avg_retransmissions, int packets_delivered, bool initial_phase)
{
	if (!initial_phase)
	{
		std::fstream file("./statistics/avgarge_number_of_retransmissions_sim_nr_" + std::to_string(simulation_nr_) + ".txt", std::ios_base::app);
		std::string line;
		line = std::to_string(avg_retransmissions);
		if (file.is_open())
		{
			file << line << "\n";
		}
		file.close();

		std::fstream file2("./statistics/avgarge_number_of_retransmissions_packets_sim_nr_" + std::to_string(simulation_nr_) + ".txt", std::ios_base::app);
		
		line = std::to_string(packets_delivered);
		if (file2.is_open())
		{
			file2 << line << "\n";
		}
		file.close();
	}
}

void Simulator::resetParameters(Event::EventList *event_list,WirelessNetwork *wireless_network)
{
	clock_ = 0;
	while (!event_list->empty())
	{
		Event *ev = event_list->top();
		event_list->pop();
		delete(ev);
	}
	delete(wireless_network);
}

//This method loads seeds from a file and puts them into vectors
void Simulator::loadSeeds()
{
	for (int i = 0; i < 10; ++i)
	{
		auto vector = new std::vector<int>;
		seeds_.push_back(vector);
	}
	int counter = 0;
	int simulation_nr = 1;
	std::string::size_type sz;
	std::string line;
	std::fstream file("./seeds.txt");
	if (file.is_open())
	{
		while (std::getline(file, line, '\n'))
		{
			int num = std::stoi(line, &sz);
			seeds_[simulation_nr - 1]->push_back(num);
			counter++;
			if (counter % 166 == 0) //because of 166 generators 
			{
				counter = 0;
				simulation_nr++;
			}
		}
		file.close();
	}
}

void Simulator::init()
{
	if (simulation_nr_ == 0)
	{
		simulation_nr_ = 1;
		number_of_simulations_ = 10;
	}
}

void Simulator::startSimulation(double end_condition, double initial_phase)
{
	int packets_delivered = 0;
	int packets_to_deliver = 0;
	int end_of_initial_phase_packets = 0;

	size_t end_of_simulation = 0;
	size_t end_of_initial_phase = 0;

	//Sets end of simulation conditions 
	if (simulation_type_ == 1)
	{
		end_of_simulation = static_cast<size_t>(end_condition * 10000);	    //10^-4[s]
		end_of_initial_phase = static_cast<size_t>(initial_phase * 10000);	// 10^-4[s]
	}
	else
	{
		packets_to_deliver = static_cast<int>(end_condition);
		end_of_initial_phase_packets = static_cast<int>(initial_phase);
	}

	auto cmp = [](Event* left, Event* right) { return left->get_time() > right->get_time(); };
	Event::EventList *event_list = new Event::EventList(cmp);	//event calendar
	
	bool step_by_step = false;
	bool channel_state = true;
	size_t previous_clock_value = 0; //it's used to prevent from checking channel state more than once in the same time unit
	
	//Ask for the step by step mode only if debug level was selected
	if (level_ == Logger::Level::Debug)
	{
		char choice = question();
		while (choice != 'n' && choice != 'y')
		{
			choice = question();
		}
		if (choice == 'y')
			step_by_step = true;
		else
			step_by_step = false;
	}

	while (simulation_counter_ <= number_of_simulations_)
	{

		logger_->Info("Simulation nr: " + std::to_string(simulation_nr_) + " started");
		WirelessNetwork *wireless_network = new WirelessNetwork(kL_, kK_, logger_, event_list, seeds_[simulation_nr_ - 1]);
		
		while ((clock_ < end_of_simulation && simulation_type_ == 1) || (packets_delivered <= packets_to_deliver && simulation_type_==2))
		{

			previous_clock_value = clock_;

			Event *event = event_list->top();
			event_list->pop();
			clock_ = event->get_time();

			
			//Check if the step by step mode was chosen
			if (step_by_step)
			{
				std::cout << "Press enter to continue \n";
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			//time display
			time_info_ = static_cast<double>(clock_) / 10; //used to display time in [ms]
			logger_->Debug("Simulation time: " + std::to_string(time_info_) + " [ms]");

			event->execute();
			delete(event);
			//check if channel was already checked in this time unit 
			if (clock_ != previous_clock_value)
			{
				channel_state = wireless_network->checkChannel(); //true - channel is free   false - channel is busy
			}
			bool no_event = false;

			//turning off the initial phase
			if (wireless_network->getInitialPhaseFlag() && ((clock_ > end_of_initial_phase && simulation_type_==1) || 
				(packets_delivered>end_of_initial_phase_packets && simulation_type_ == 2)))
			{
				wireless_network->turnOffInitialPhase();
			}

			while (no_event == false)
			{
				no_event = true;
				//check state of every transmitter 
				for (int i = 0; i < kK_; ++i)
				{
					//Channel Listening Event
					if (!wireless_network->isBufferEmpty(i) && wireless_network->checkChannelListeningFlag(i) &&
						wireless_network->checkChannelCheckingEventFlag(i))
					{
						no_event = false;
						wireless_network->resetChannelListeningFlag(i);
						wireless_network->channelChecking(i, clock_, channel_state);

					}
					//Delay Event
					if (wireless_network->checkChannelListeningTime(i) == kDifs_ && wireless_network->checkDelayEventFlag(i)
						&& wireless_network->checkChannelListeningFlag(i))
					{
						no_event = false;
						wireless_network->delayEvent(i, clock_, channel_state);
					}
					//Transmission Event
					if (wireless_network->getTransmitterBt(i) == 0 && wireless_network->checkTransmissionFlag(i) &&
						wireless_network->checkChannelListeningFlag(i) && !wireless_network->checkDelayEventFlag(i))
					{
						no_event = false;
						wireless_network->packetTransmissionEvent(i, clock_);
					}
					//Check Ack Event
					if (wireless_network->checkAckEventFlag(i))
					{
						no_event = false;
						wireless_network->resetTransmitterAckEventFlag(i);
						if (wireless_network->getReceiverAckFlag(i))
						{
							//Transmission successful, delete packet from buffer

							wireless_network->incrementPacketsCounter(i);
							wireless_network->setSuccessfulTransmissionFlag(i);
							wireless_network->setDiscardPacketEventFlag(i);
							packets_delivered++;
							saveAvgNumberOfRetransmissions(wireless_network->calculateAvgNumberOfRetransmissions(),
								wireless_network->getPacketsDelivered(), wireless_network->getInitialPhaseFlag());
						}
						else
						{
							//Transmission failed
							wireless_network->setRetransmissionEventFlag(i);
						}
					}
					//Retransmission Event
					if (wireless_network->checkRetransmissionEventFlag(i))
					{
						no_event = false;
						wireless_network->retransmissionEvent(i, clock_);
					}
					//Discard Packet Event
					if (wireless_network->checkDiscardPacketEventFlag(i))
					{
					
						no_event = false;
						size_t execution_time = clock_ + static_cast<size_t>(5);
						wireless_network->discardPacketEvent(i, execution_time,clock_);
						savePacketErrorRate(wireless_network->calculatePacketErrorRate(), clock_, wireless_network->getInitialPhaseFlag());
					}
				}
			}

		}
		//Results
		logger_->Info("End of simulation nr: " + std::to_string(simulation_nr_));
		logger_->Info("RESULTS:");
		logger_->Info("Avarge packet error rate: " + std::to_string(wireless_network->calculatePacketErrorRate()));
		logger_->Info("Maximal packet error rate: " + std::to_string(wireless_network->getMaximalPacketErrorRate()));
		logger_->Info("Avarge number of retransmissions: " + std::to_string(wireless_network->calculateAvgNumberOfRetransmissions()));
		logger_->Info("Throughput: " + std::to_string(wireless_network->calculateThroughput(clock_)) + " [packets/s]");
		logger_->Info("Avarge packet delay: " + std::to_string(wireless_network->calculateAvgDelay()) + " [ms]");
		logger_->Info("Avarge waiting time: " + std::to_string(wireless_network->calculateAvgWaitingTime()) + " [ms]");
		saveResults(wireless_network);
		resetParameters(event_list,wireless_network);
		simulation_counter_++;
		simulation_nr_++;
		packets_delivered = 0;
	}
	




}
