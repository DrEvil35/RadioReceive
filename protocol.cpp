#include "protocol.h"



Protocol::Protocol(const String& sign, uint8_t tolerance){
	this->sign = sign;
	cb = [](Protocol*){};
	this->tolerance = tolerance;
	reset();
}

Protocol::~Protocol(){
	
}

const String& Protocol::getSign(void){
	return std::move(this->sign);
}

Protocol::State Protocol::getState(void){
	return this->state;
}

void Protocol::onReceive( const cbProtocol cb ){
	this->cb = cb;
}

uint64_t Protocol::getPacket(void){
	return this->packet;
}

void Protocol::reset(){
	this->bit_count = 0;
	this->packet = 0;
	this->state = State::WAIT;
}

bool Protocol::isTolerance(uint16_t& dur1, uint16_t& dur2){
	auto diff = abs(dur1, dur2);
	return diff > this->tolerance ? false : true;
}

void Protocol::setRepeatReceive(uint8_t repeat){
	this->lp_repeat = repeat;
}

void Protocol::setPauseReceive(uint16_t pause){
	this->max_pause = pause;
}

void Protocol::setTresholdMin(uint16_t tresh){
	this->min_treshold = tresh;
}

void Protocol::printRaw(void){
	printBinary(this->packet);
}

void Protocol::receiveData(uint16_t& duration){
	if(this->state == AVAILABLE || duration < this->min_treshold || !processPacket(duration)){
		return;
	}
	auto time = millis();
	if(this->lp_repeat && time - this->lp_time < this->max_pause && this->lp_data == this->packet){
		this->lp_counter++;
	}
	if(this->lp_repeat == 0 || this->lp_counter >= this->lp_repeat){
		this->lp_counter = 0;
		this->cb(this);
		return;
	}
	this->lp_time = time;
	this->lp_data = this->packet;
	reset();
}

bool Protocol::processPacket(uint16_t& duration){
	
}



