#ifndef RADIO_H_
#define RADIO_H_

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#include "protocol.h"



class Radio{
	typedef std::function<void(Radio&, uint32_t, uint8_t)> ISRCallback;
private:
	Protocol** protocols;
	uint8_t interrupt;
	uint8_t protocolos_size;
	uint32_t lastISR;
	ISRCallback callback;
	void isr();

	
public:
	Radio(uint8_t interrupt, size_t plugins);
	~Radio();
	void enable(void);
	void disable(void);
	void listen(void);
	void onISR(const ISRCallback callback);
	void registryProtocol(const Protocol& proto);
	void resetProtocol();
}; 

#endif
