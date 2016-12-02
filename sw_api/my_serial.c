#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#define CPU_FOSC 16000000
#define GET_8_UBRR(baud) CPU_FOSC/8/baud - 1

///\brief init USART0, 8 bits, 1 stop bit, no parity frame
void serial0_init(unsigned int baud){
	//~ cli(); ///< clear global interrupts
//~ 
	//~ UCSR0A = 0x20; ///< Ensure default value, UDR empty
	//~ UCSR0B = 0x0; ///< Ensure default value, all ints disable 		
	//~ UCSR0C = 0x0 ///< Ensure UCSZ0 only the selected ones	
		//~ 
		//~ ///\briefwait for any ongoing transmission to stop
		//~ ///TXCN and/or RXCN
	//~ //while((UCSR0A &(1<<RXC0)) || (UCSR0A &(1<<TXC0)));
	//~ 
	//~ UBRR0 = GET_8_UBRR(baud); ///< calc UBRR value
	//~ UCSRC0 |= (1<<UCSZ01)///< Config frame 8bits, 1stopbit, no parity
	//~ UCSRA0 |= (1<<U2X0); ///< Double speed, 8 factor
	//~ UCSRB0 |= (1<<RXEN0)|(1<<TXEN0);///< Enable TX and RX
	//~ 
	//~ sei();///< reset global interrupts
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<USBS)|(3<<UCSZ0);
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
