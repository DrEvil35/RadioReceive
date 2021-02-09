#ifndef RADIO_PROTO_RCSWITCH_H_
#define RADIO_PROTO_RCSWITCH_H_

#include "protocol.h"


class PTProtocol: public Protocol{
private:
	enum pulseState{ PAUSE, RECEIVE } pulse_state = PAUSE;
	uint16_t pulse_width;
	uint16_t last_duration;
	uint8_t size_tresh;
	uint8_t tick;
	const static uint16_t min_pulse_width = 1200;
	const static uint16_t max_pulse_width = 2500;
public:
	PTProtocol(const String& sign, uint8_t size_tresh = 16 );
	bool processPacket( uint16_t& duration) override;
	uint8_t getSize(void);
};

#endif

