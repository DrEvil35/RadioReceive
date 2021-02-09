#include "ptprotocol.h" 


PTProtocol::PTProtocol(const String& sign, uint8_t size_tresh) : Protocol(sign, 50){
	this->bit_size = 64;
	this->size_tresh = size_tresh;
	setTresholdMin(350);
}

uint8_t PTProtocol::getSize(void){
	return this->bit_count;
}

bool PTProtocol::processPacket( uint16_t& duration){
	uint16_t width = duration + this->last_duration;
	this->last_duration = duration;
	if(this->state == State::WAIT){
		this->packet <<= 1; 
		uint8_t sync_bit = duration < this->last_duration ? 1 : 0;
		if(this->bit_count == 0){ 
			if(sync_bit == 1){ 
				return false;
			}
			if( width < min_pulse_width || width > max_pulse_width ){
				reset();
				return false;
			}
			this->pulse_state = PAUSE;
			this->pulse_width = width;
			this->bit_count++;
			return false;
		}else{ // check first an 2/3/4 sync bit and set pulse width
			if(this->pulse_state == PAUSE){
				this->pulse_state = RECEIVE;
				return false;
			}else{
				if(isTolerance(this->pulse_width, width) && sync_bit == 0){
					this->pulse_state = PAUSE;
					this->pulse_width += width;
					this->pulse_width /= 2;
					this->bit_count++;
					if(this->bit_count == 4){
						this->state = SYNC;
						this->tick = this->pulse_width / 16;
						return false;
					}
				}else{
					reset();
					return false;
				}
			}
		}
	}else{

		if(this->pulse_state == PAUSE){
			this->pulse_state = RECEIVE;
		}else{
			this->pulse_state = PAUSE;
			if(!isTolerance( this->pulse_width, width ) ){
				if(this->bit_count > this->size_tresh){
					this->state = AVAILABLE;
					return true;
				}else{
					reset();
				}
				return false;
			}
			if(this->bit_count == this->bit_size){
				reset();
			}else{
				uint16_t data_ticks = abs(duration, width)/this->tick;
				this->packet <<= 1;
				if(11 == data_ticks || data_ticks == 12){
					this->packet|=1;
				}
				this->bit_count++;
			}
		}
	}
	return false;
}


