#include "AckCheckEvent.h"

AckCheckEvent::AckCheckEvent(Transmitter * transmitter, int execution_time,Logger *logger):
	transmitter_(transmitter),Event(execution_time),logger_(logger)
{
	logger_->Debug("Created a new AckCheckEvent");
	double time_info = static_cast<double>(execution_time) / 10;
	logger_->Debug("Transmitter " + std::to_string(transmitter_->get_id()) + ": Created a new AckCheckEvent execution time: " + std::to_string(time_info) + " [ms]");
}

void AckCheckEvent::execute()
{
	transmitter_->setAckEventFlag(true);

}
