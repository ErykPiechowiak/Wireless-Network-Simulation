#ifndef PACKET_H_
#define PACKET_H_
#include <random>
//This class represents packet which is created by transmitter 
class Packet
{
public:
	Packet(int id,int ctp);
	~Packet();
	void failedTransmission();//method used by channel if there was a collision
	int get_id();
	int get_ctp();
	bool getIsCorrectFlag();
	void set_ter(bool ter); //if ter is false value 'is_correct' will be also false
	void setAddedToBufferTime(size_t time);
	size_t getAddedToTheBufferTime();
private:
	int id_;
	int ctp_; //transmission time 
	bool is_correct_; //True if packet will be delivered successfully, otherwise false
	size_t added_to_buffer_time_;


};
#endif // !PACKET_H_



