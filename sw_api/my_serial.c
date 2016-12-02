#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#define CPU_FOSC 16000000

unsigned int get_ubrr(unsigned int baud, unsigned char u2xsss){
	return CPU_FOSC/u2x/baud - 1;
}

///\brief init USART0, 8 bits, 1 stop bit, no parity frame
void serial0_init(unsigned int baud){
	cli(); ///< clear global interrupts
	while(UCSR0A&(1<<TXC0));///< Avoid change baud or frame in current tranmission, if exits
	UBRR0 = get_ubrr(9600, 0); ///< stp 1. set baud rate
	
	///<Ensure restart from defaults
	UCSR0A = 0x20; ///< Only UDR empty bit enable, no double speed, reset error flags
	UCSR0B = 0x0; ///< Disable all interrupts
	UCSR0C = 0X00; ///< Reset frame format and prepare for 8 bit long, async, no parity, one stop bit, rising edge
	
	///< stp 2. Set frame format		
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01) ///< Ensure 8 bit long
	///< stp 3. Enable TX and RX
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0); ///< And let the UCSZ02 as zero, for the 8b frame

	sei(); ///< reset global interrupts
}


///\brief send serial data by polling
void serial_send(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0))); ///< Wait for UDR available
	//~ UCSR0A ~= (1<<TXC0); //Clear TX before write, as suggested in datasheet
	UDR0 = data; ///< Only put data into the reg, not sent, since it takes more time
}

///\brief receive serial data by polling
unsigned char serial_receive(){
	while(!(UCSR0A & (1<<RXC0))); ///< wait for incoming data
	return UDR0; ///< return character
}
