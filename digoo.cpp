#include "digoo.h"

DigooWeather::DigooWeather(const String& sign) : Protocol(sign, 10){
	this->bit_size = 36;
	setTresholdMin(650);
}

float DigooWeather::getTemperature(void){
	int16_t t = this->packet >> 12 & 0x0FFF;
	t = 0x0800 & t ? 0xF000 | t  : t;
	return float(t) / 10;
}

uint8_t DigooWeather::getHumidity(void){
	return this->packet & 0xFF;
}

bool DigooWeather::getBatteryStatus(void){
	return ( this->packet >> 24 );
}

uint8_t DigooWeather::getIdSensor(void){
	return ((this->packet >> 24) & 0x3)+1;
}

bool DigooWeather::processPacket(uint16_t& duration ){
	if( this->state == State::WAIT && ( duration > 3000 && duration < 4000 )){
		reset();
		this->state = State::SYNC;
		return false;
	}
	packet <<= 1;
	this->bit_count++;
	if(bit_count < 5){
		if(bit_count != 1 && !isTolerance(this->low_duration, duration)){
			reset();
			return false;
		}
		this->low_duration = duration;
		return false;
	}
	if(bit_count < 9){
		if(bit_count!=5 && !isTolerance(this->high_duration, duration)){
			reset();
			return false;
		}
		this->high_duration = duration;
		this->packet |= 1;
		return false;
	}
	if( isTolerance(this->high_duration, duration)){
		this->packet |= 1;
	}
	else if( isTolerance(this->low_duration, duration)){

	}
	else{
		reset();
		return false;
	}
	if( this->bit_count == this->bit_size ){
		this->state = State::AVAILABLE;
		return true;
	}
	return false;
}
