#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "WirelessNetwork.h"
#include "Logger.h"
class Simulator
{
public:
	explicit Simulator(const int K, const double L, Logger *logger, int simulation_nr, int symulation_type, Logger::Level level);
	void startSimulation(double simulation_time,double initial_phase_time );
private:
	void savePacketErrorRate(double packet_error_rate, size_t time, bool initial_phase);
	void saveResults(WirelessNetwork *wireless_network);
	void saveAvgNumberOfRetransmissions(double avg_retransmissions, int packets_delivered, bool initial_phase);
	void resetParameters(Event::EventList *event_list, WirelessNetwork *wireless_network);
	void loadSeeds();
	void init();

	const int kK_;		// number of transmitters and receivers 
	const double kL_;	//intensivity of exponential distribution
	const int kDifs_ = 30; //time of a channel listening / 30*10^-4[s]
	int number_of_simulations_ = 1;
	int simulation_nr_ = 1;
	int simulation_counter_ = 1; // used when user decided to run 10 simulations 
	int simulation_type_; //selected by user 1) max simulation time  2) Number of delivered packets
	size_t clock_ = 0;
	Logger *logger_ = nullptr;
	Logger::Level level_;
	std::vector<std::vector<int>*> seeds_; //each vector contains 166 seeds (for one simulation)
	double time_info_;		//time info is used to display informations about current clock value in [ms]


	//statistics
	double packet_error_rate_ = 0;

};

#endif // !SIMULATOR_H_


