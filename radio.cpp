#include "radio.h"

#if  defined(ESP8266) || defined(ESP32)
	#include "FunctionalInterrupt.h"
#endif

#if defined(ESP8266)
    #define RECEIVE_ISR ICACHE_RAM_ATTR
#elif defined(ESP32)
    #define RECEIVE_ISR IRAM_ATTR
#else
    #define RECEIVE_ISR
#endif




Radio::Radio(uint8_t interrupt, size_t size){
	this->interrupt = interrupt;
	this->protocolos_size = size;
	this->callback = [](Radio&,uint32_t,uint8_t){};
	this->protocols =  new Protocol*[size];
	for(uint8_t i=0; i<this->protocolos_size; i++){
		this->protocols[i] = nullptr; };
}

Radio::~Radio(){
	this->disable();
}

void Radio::enable(void){
	pinMode(this->interrupt, INPUT);
	digitalWrite(this->interrupt, LOW);
	attachInterrupt(this->interrupt,
					std::bind(&Radio::isr,this),
					CHANGE );
}

void Radio::resetProtocol(){
	for(uint8_t i=0; i<this->protocolos_size; i++){
		Protocol* p = this->protocols[i];
		if(p != nullptr){
			p->reset();
		}
	};
}

void Radio::disable(void){
	detachInterrupt(this->interrupt);
}

void Radio::registryProtocol(const Protocol& proto){
	for(uint8_t i=0; i<this->protocolos_size; i++){
		auto* p = &this->protocols[i];
		if(*p == nullptr){
			*p = (Protocol*)&proto;
			break;
		}
	};
}

void Radio::onISR(const ISRCallback callback){
	this->callback = callback;
}

RECEIVE_ISR void Radio::isr(void){
 	auto time = micros();
 	uint16_t duration = time - this->lastISR;
 	this->lastISR = time;
	uint8_t state = digitalRead(this->interrupt);
	for(uint8_t i=0; i<this->protocolos_size; i++){
		Protocol* p = this->protocols[i];
		if(p == nullptr)
			break;
		p->receiveData(duration);
	};
	this->callback( *this, duration, state);
}
