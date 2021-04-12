#include "ChannelListeningEvent.h"

ChannelListeningEvent::ChannelListeningEvent(size_t execution_time,Transmitter * transmitter, Logger *logger):
	Event(execution_time),transmitter_(transmitter), logger_(logger)
{
	time_info_ = static_cast<double>(execution_time) / 10;
	logger_->Debug("Transmitter "+std::to_string(transmitter_->get_id())+": Created a new ChannelListeningEvent");
	logger_->Debug("ChannelListeningEvent execution time: " + std::to_string(time_info_) + " [ms]");
}

void ChannelListeningEvent::execute()
{
	transmitter_->setChannelListeningFlag(true);
}
