#include "Logger.h"
#include <iostream>


Logger::Logger()
{
	
}

Logger::~Logger()
{
}

void Logger::Info(std::string message)
{
	if (level_ != Level::Error)
	{
		std::cout << "[Info] " << message << std::endl;
	}
	
}

void Logger::Debug(std::string message)
{
	if (level_ != Level::Error && level_ != Level::Info)
	{
		std::cout << "[Debug] " << message << std::endl;
	}
}

void Logger::Error(std::string message)
{

	
	std::cout << "[Error] " << message << std::endl;
	

}
