# Wireless-Network-Simulation

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
The application is a simulation of a wireless network based on the CSMA/CA protocol. 
Used simulation method: ABC.

Simulation results:
* Avarge packet error rate for each transmitter
* Avarge number of retransmissions
* System throughput in [packets/sec]
* Avarge packet delay
* Avarge packet waiting time

The application was developed in June 2020.

## Technologies

C++

## Setup

**Program calling parameters**

* L - lambda parameter which is the intensity of the exponential distribution used by packets generators
* Simulation number (1-10) - each simulation has different set of seeds used by random generators 
* End of simulation condition (1 - max simulation time; 2- number of delivered packets) 
* End of initial phase

**Fixed parameters that can be changed in code**

* Simulation mode (Debug/Info)
* K - number of transmitters and receivers
* TER - chance for an error while transmitting packet
* CWmin, CWmax - contention window parameters 

