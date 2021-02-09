#ifndef RADIO_PROTO_H_
#define RADIO_PROTO_H_

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#include <limits.h>

#define bitset(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define bitclear(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define bitflip(byte,nbit)  ((byte) ^=  (1<<(nbit)))
#define bitcheck(byte,nbit) ((byte) &   (1<<(nbit)))
 
template<typename T>
void printBinary(T value)
{
    for ( size_t mask = 1 << ((sizeof(value) * CHAR_BIT) - 1); mask; mask >>= 1 )
    {
        Serial.print(value & mask ? "1" : "0");
    }
}

template<typename T>
inline T abs(T& arg1, T& arg2){
	return arg1 > arg2 ? arg1 - arg2 : arg2 - arg1;
}
 
class Protocol {
	typedef std::function<void(Protocol*)> cbProtocol;

	public:
		enum State { WAIT, SYNC, AVAILABLE };
		Protocol(const String& sign, uint8_t tolerance = 0);
		virtual ~Protocol();
		const String& getSign(void);
		Protocol::State getState(void);
		void onReceive( const cbProtocol cb );
		uint64_t getPacket(void);
		void reset(void);
		void printRaw(void);
		void receiveData(uint16_t& duration);
		bool isTolerance(uint16_t& dur1, uint16_t& dur2);
		void setRepeatReceive(uint8_t repeat);
		void setPauseReceive(uint16_t pause);
		void setTresholdMin(uint16_t tresh);
	
	protected:
		String sign;
		cbProtocol cb;
		uint64_t packet;
		uint8_t bit_size;
		uint8_t bit_count;
		Protocol::State state;
		virtual bool processPacket( uint16_t& duration);
		
	private:
		uint16_t min_treshold;
		uint16_t max_pause;
		uint8_t tolerance;
		uint8_t lp_repeat;
		uint8_t lp_counter;
		uint64_t lp_data;
		uint32_t lp_time;
} ;

#endif
