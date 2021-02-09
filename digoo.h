#ifndef RADIO_PROTO_DIGOO_H_
#define RADIO_PROTO_DIGOO_H_

#include "protocol.h"

// 15:17:20.437 -> Temp: 300
// 15:17:20.437 -> ID: 3
// 15:17:20.437 -> HM: 32
// 15:17:20.437 -> 1111 10 10 000100101100 1111 00100000
// 				1111 10 10 111111001111 1111 00011011 
// 				1111 10 10 000000001101 1111 00101011
// [4b header] [2bit ?] [2 bit id] [12 bit temp] [4bit ?] [8bit hm]

class DigooWeather: public Protocol{

private:
	uint16_t low_duration;
	uint16_t high_duration;

public:
	DigooWeather(const String& sign);
	bool processPacket( uint16_t& duration ) override;
	float getTemperature(void);
	uint8_t getHumidity(void);
	bool getBatteryStatus(void);
	uint8_t getIdSensor(void);
};

#endif
