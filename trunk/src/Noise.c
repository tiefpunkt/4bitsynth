#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include "Noise.h"


void init_interrupts(){
	//Turn on reception and | RX Interrupt
	UCSR0B = (1 << RXEN0)|(1<<RXCIE0);

	//8-bit, 1 stop, Asynch.
	UCSR0C = (0<<UMSEL00) | (0<<UPM00) | (0<<USBS0) | (3<<UCSZ00);

	/* Set the baud rate to 31250 for MIDI */
	UBRR0L = 0x27;	// For 20MHz Clock
	//UBRR0L = 0x13;		// FOr 10MHz Clock

	/* Enable USART Receive interrupt */
	enable_USART_interrupts();

	/* Enable falling edge interrupt on INT0 */
	/* ISC00 = 0; ISC01 = 1; */
	//EICRA = 0x02;

	/* Turn on INT0 interrupt */
	/* EIMSK = -----001 */
	//EIMSK = 1;
	//PCMSK3 = 0;

	// Enable Timer Overflow Interrupt
	//TIMSK1 = 1;
}



void init_io(){
	DDRC = 0x0F;
	DDRD &= 00001111;
	PORTD |= 0b11110000;

	DDRB |= 0b00001111;
	
}


void init_timers(){
	//8-bit timer 0 for decay, sweep, vibrato effects?
	TIMSK0 = 0b00000001;	//Enable Overflow interrupts for Timer 0
	TCCR0A = 0b00000000;	//Normal counter operation
	TCCR0B = 0b00000101;	//Divide by 1024 prescalar
	TCNT0 = 0x00;			//Start terminal count at zero

	//16-bit timer 1 for main frequency generation
	TIMSK1 = 0b00000010;	// Enable A compare interrupts
	
	TCCR1A = 0b00000001;
 	TCCR1B = 0b00010010;	// Prescaler 8, Fast PWM

	
	TCNT1H = 0;
	TCNT1L = 0;
}


int main(void)
{
	/* Disable interrupts at first */
	cli();

	/* Setup I/O Pins */
	init_io();

	/* Setup Timers */
	init_timers();

	/* Enable USART and Pin Interrupts */
	init_interrupts();

	num_bytes = 0;
	num_ccs = 0;
	num_pbs = 0;
	byte_ready = 0;
	midi_channel = 0;
	master_volume = 127;

	OCR1A = 25;

	tap_one = 13;
	tap_two = 14;
	shift_register = 0b01101001100101011; //Seed shift register
	fake_8_timer = 0;

	amplitude = 0;
	voldecay_enabled = 1;
	voldecay_amount = 0;
	voldecay_loop_enabled = 0;
	fine_pitch_bend = 0;

	sweep_enabled = 0;
	sweep_direction = SWEEP_DOWN;
	sweep_amount = 0;
	sweep_loop_enabled = 1;

	/* Finally, enable global interrupts */
	sei();

	/*Main Loop*/
	while(1){
		check_channel_set();
		check_byte_received();
		clock_shift_register();
	}
	return 0;
}

/* USART Received byte interrupt (get MIDI byte)*/
ISR(USART_RX_vect){
	byte_received = UDR0;
	byte_ready = 1;
}

ISR(TIMER1_COMPA_vect)
{
	clock_ready = 1;
}

ISR(TIMER0_OVF_vect)
{
	
	/* Decay envelope */
	if(voldecay_enabled == 1){
		fake_8_timer ++; //Count a fake timer to artificially slow down real timer
		if(fake_8_timer > 8){
			fake_8_timer = 0;
			if(amplitude >= voldecay_amount)
				amplitude -= voldecay_amount;
			else
				amplitude = 0;
		}
	}

	/* Sweep */
	if((sweep_enabled == 1) && (sweep_amount > 0)){
		//unsigned int sweep_mod = sweep_amount * (note_table[playing_midi_note] / (8192/(current_midi_note*2)));
		unsigned int sweep_mod =  sweep_amount * note_table[playing_midi_note] / (8 * playing_midi_note);
		//Sweep down mode
		if(sweep_direction == SWEEP_DOWN){
			if(frequency < MAXIMUM_FREQ){
				frequency += sweep_mod;
			}
			else
				{
					if(sweep_loop_enabled == 1){
						frequency = note_table[playing_midi_note];
					}
					else {
						amplitude = 0;
					}
				}
		}
		//Sweep up mode
		else
		{
			if(frequency > MINIMUM_FREQ+sweep_mod){
				frequency -= sweep_mod;
			}
			else
				{
					if(sweep_loop_enabled == 1){
						frequency = note_table[playing_midi_note];
					}
					else {
						amplitude = 0;
					}
				}
		}
		
			update_frequency(frequency);
	}

}

void check_byte_received(){

	//Is there a byte waiting in the buffer?
	if(byte_ready == 1){
		//Is this a stupid byte like Clock or Active sensing?
		if(byte_received < 0xF0)
		{
			//Is this a status byte? ...
			if(byte_received >= 0x80)
			{
				unsigned char temp_midi_channel = byte_received & 0x0F;
				//Is this for one of our channels?
				if(temp_midi_channel == midi_channel)
				{
					current_midi_channel = temp_midi_channel;

					//What kind of status byte is this?
					unsigned char status_type = (byte_received & 0xF0);
					switch(status_type)
					{
					case(MIDI_STATUS_TYPE_NOTEON):
						current_midi_status = MIDI_STATUS_NOTEON;
					break;
					case(MIDI_STATUS_TYPE_NOTEOFF):
						current_midi_status = MIDI_STATUS_NOTEOFF;
					break;
					case(MIDI_STATUS_TYPE_CC):
						current_midi_status = MIDI_STATUS_CC;
					break;
					case(MIDI_STATUS_TYPE_PB):
						current_midi_status = MIDI_STATUS_PB;
					break;
					default:
						current_midi_status = MIDI_STATUS_NONE;
					}
				} else {
					current_midi_status = 0;
				}
			}
			// ... or is it a data byte?
			else
			{
				switch(current_midi_status)
				{
				case(MIDI_STATUS_NOTEON):

					//Is this a velocity byte?
					if(num_bytes > 0)
					{
						
						//If the velocity sent was 0, then this is really a NOTE-OFF
						if(byte_received > 0){
							current_midi_velocity = byte_received;
							playing_midi_note = current_midi_note;
							note_on();
						}
						else{
							if(current_midi_note == playing_midi_note) {
								current_midi_velocity = 0;
								note_off();
							}
						}
						num_bytes = 0;
					}

				//Or is this a note data byte?
					else {
						current_midi_note = byte_received;
						num_bytes ++;
					}

				//Clear the byte so we don't process it twice
				clear_byte_received();

				break;

				case(MIDI_STATUS_NOTEOFF):
					//Is this a velocity byte?
					if(num_bytes > 0){
						//Who cares? We aren't implementing aftertouch
						num_bytes = 0;
					} else {
						/* This is a note byte. Let's see if it's the same as the currently
						 * playing note. Only then will we note_off()
						 */

						if(byte_received == playing_midi_note)
						{
							note_off();
						}
						num_bytes ++;
					}

				//Clear the byte so we don't process it twice
				clear_byte_received();

				break;

				case(MIDI_STATUS_CC):
					//Did we already get a CC Status byte?
					if(num_ccs > 0)
					{
						current_midi_ccdata = byte_received;
						process_cc();
					}
				//Or is this a new CC status byte?
					else
					{
						current_midi_cc = byte_received;
						num_ccs ++;
					}
				break;

				case(MIDI_STATUS_PB):
					//How many PB related bytes have we gotten?
					switch(num_pbs)
					{
					case(0):
						//First byte is 7 LSB
						//Don't care about it for now
						//current_midi_pb_l = byte_received;
						
						num_pbs ++;
					break;
					case(1):
						//Second byte has 7 MSB
						current_midi_pb_h = byte_received;
						//Combine to get 14 bytes 0 - 13
						//current_midi_pb = ((current_midi_pb_h << 7)|(current_midi_pb_l << 0));
						bend_pitch();

					break;
					}
				break;
				}
			}

		}
		byte_ready = 0;
	}
}


void enable_USART_interrupts(){
	UCSR0A = (1<<RXCIE0);
}


void disable_USART_interrupts()
{
	UCSR0A = (0<<RXCIE0);
}


void note_on()
{
	num_bytes = 0;
	TCNT1 = 0;

	frequency = note_table[current_midi_note];
	update_frequency(frequency);
	if(current_midi_velocity > master_volume)
		amplitude = master_volume/8;
	else
		amplitude = current_midi_velocity/8;


}


void note_off()
{
	num_bytes = 0;
	amplitude = 0;
}


void process_cc()
{
	num_ccs = 0;

	switch(current_midi_cc){
		case NOISE_TYPE_CC:
			if(current_midi_ccdata > 64){
				//64k mode
				tap_one = 8;
				tap_two = 14;
			}
			else {
				//93-bit mode
				tap_one = 14;
				tap_two = 13;
			}
			break;
		case MASTER_VOLUME_CC:
			master_volume = current_midi_ccdata;
			break;
		case FINE_PITCH_CC:
			fine_pitch_bend = ((note_table[playing_midi_note - 1] - note_table[playing_midi_note]) * current_midi_ccdata) / 192;
			break;
		case VOLDECAY_ENABLED_CC:
			if(current_midi_ccdata > 64)
				voldecay_enabled = 1;
			else
				voldecay_enabled = 0;
			break;

		case VOLDECAY_AMOUNT_CC:
			voldecay_amount = current_midi_ccdata/8;
			break;

		case SWEEP_ENABLED_CC:
			if(current_midi_ccdata > 64)
				sweep_enabled = 1;
			else
				sweep_enabled = 0;
			break;

		case SWEEP_DIRECTION_CC:
			if(current_midi_ccdata > 64)
				sweep_direction = SWEEP_UP;
			else
				sweep_direction = SWEEP_DOWN;
			break;

		case SWEEP_AMOUNT_CC:
			sweep_amount = current_midi_ccdata;
			break;
		case SWEEP_LOOP_ENABLED_CC:
			if(current_midi_ccdata > 64)
				sweep_loop_enabled = 1;
			else
				sweep_loop_enabled = 0;
			break;
	
	}
}

void update_frequency(unsigned int new_frequency)
{
	OCR1A = new_frequency + fine_pitch_bend;
}


void bend_pitch()
{
		num_pbs = 0;
	
		if(current_midi_pb_h > 63){
			distance = ((note_table[playing_midi_note]-note_table[playing_midi_note+2])*(current_midi_pb_h - 63))/64;
			update_frequency(note_table[playing_midi_note] - distance);
		}
		else if((current_midi_pb_h < 63) && (playing_midi_note > 1)){
			distance = ((note_table[playing_midi_note - 2] - note_table[playing_midi_note]) * (64 - current_midi_pb_h))/64;
			update_frequency(note_table[playing_midi_note] + distance);
		}


}

void clock_shift_register(){
	if(clock_ready > 0){
		shift_register = (shift_register << 1);	//The "shifting operation"
		unsigned char bitone = ((shift_register & (1 << tap_one)) >> tap_one);
		unsigned char bittwo = ((shift_register & (1 << tap_two)) >> tap_two);
		shift_register |= ((bitone ^ bittwo));
		PORTC &= 0xF0;
		PORTC |= (shift_register & 1) * amplitude;
		clock_ready = 0;
	}
}

void clear_byte_received(){
	byte_received = 0;
}

void check_channel_set(){
	midi_channel = 0;
	midi_channel |= (~PIND & 0xF0) >> 4;
	PORTB = ~midi_channel;
}

