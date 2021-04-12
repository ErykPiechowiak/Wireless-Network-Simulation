/*
This program simulates wireless network working with CSMA/CA protocol

Simunation method: ABC
Author: Eryk Piechowiak
*/

#include <iostream>
#include <random>
#include <vector>
#include "Simulator.h"
#include "Logger.h"

int packetsInput(std::string message)
{
	bool temp = false;
	std::string input;
	std::string::size_type sz;
	int packets; 
	while (!temp)
	{
		std::cout << message;
		std::cin >> input;
		temp = true;
		for (int i = 0; i < input.length(); ++i)
		{
			if (input.length() > 1 && input[0] == '0')
			{
				std::cout << "Invalid input \n";
				temp = false;
				break;
			}
			else if (!isdigit(input[i]))
			{
				std::cout << "Invalid input \n";
				temp = false;
				break;
			}
		}
	}
	packets = std::stoi(input, &sz);
	return packets;
}
double timeInput(std::string message)
{

	bool temp = false;
	std::string input;
	std::string::size_type sz;
	double simulation_time;
	while (!temp)
	{
		std::cout << message;
		std::cin >> input;
		temp = true;
		bool first_dot = true;
		for (int i = 0; i < input.length(); ++i)
		{
			if (!(isdigit(input[i])) || (input[i] == '.'))
			{
				if (input[0] == '.' || input[input.length() - 1] == '.')
				{
					temp = false;
					break;
				}
				if (first_dot == true && input[i] == '.')
				{
					first_dot = false;
				}
				else
				{
					temp = false;
					break;
				}
			}
		}	
	}
	simulation_time = std::stod(input, &sz);
	return simulation_time;
}

int validateEndOfSimulationCondition()
{
	std::cout << "Choose end of simulation condition \n";
	std::cout << "1) Max simulation time \n";
	std::cout << "2) Number of delivered packets \n";

	bool temp = false;
	int output;
	std::string::size_type sz;
	std::string input; 
	while (!temp)
	{

		std::cin >> input;
		if (input.length() == 1 && isdigit(input[0]))
		{
			output = std::stoi(input, &sz);
			if (output == 1 || output == 2)
			{
				break;
			}
		}
		std::cout << "Please choose proper end of simulation condition \n";
		
	}
	return output;
}
int simulationNrInput()
{
	std::cout << "Choose simulation number <1;10>, or type '0' if you want to run all of them ";
	bool temp = false;
	int simulation_nr;
	std::string::size_type sz;
	std::string input;
	while (!temp)
	{
		std::cin >> input;
		if (input.length() == 1 && isdigit(input[0]))
		{
			simulation_nr = std::stoi(input, &sz);
			if (simulation_nr >= 0 && simulation_nr < 10)
			{
				break;
			}
		}
		else if (input.length() == 2 && isdigit(input[0]) && isdigit(input[1]))
		{
			simulation_nr = std::stoi(input, &sz);
			if (simulation_nr == 10)
			{
				break;
			}
		}
		std::cout << "Please choose proper simulation number \n";

	}
	return simulation_nr;
}

int main()
{
	double L;		//intensity of exponential distribution
	const int K = 55;			//Number of transmitters and receivers 
	Logger logger = Logger();


	//Level Debug displays all informations
	//Level Info displays only simulation results
	Logger::Level level = Logger::Level::Info;
	logger.set_level(level); 

	//User input

	std::cout << "Enter L value ";
	std::cin >> L;

	int simulation_nr = simulationNrInput();
	int simulation_type = validateEndOfSimulationCondition();
	Simulator simulator(K, L, &logger, simulation_nr, simulation_type, level);
	if (simulation_type == 1)
	{
		double simulation_time = timeInput("Enter max simulation time [s] ");
		double initial_phase_time = timeInput("Enter initial phase time [s] ");
		simulator.startSimulation(simulation_time, initial_phase_time);
	}
	else
	{
		int max_packets = packetsInput("Enter a number of packets ");
		int initial_phase_packets = packetsInput("Enter initial phase packets ");
		simulator.startSimulation(max_packets, initial_phase_packets);
	}

	return 0;
}
