//Print a string to putty

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define USART_BAUDRATE 57600
#define BAUD_PRESCALE F_CPU/(USART_BAUDRATE*16)-1

//prototypes

void INIT_USART();
void outputString(char*);

int main (void)
{
	char myStr[81] = "Hello Embedded world!";
	INIT_USART();
	while(1)
	{
		
		outputString(myStr);
		_delay_ms(1000);
	}
	return 1;
}

void INIT_USART()
{
	UCSR0B |= 0x18;
	//8 bit character frames in 8 bit asynchronous mode
	UCSR0C |= 0x06;
	
	//set the baud rate
	
	UBRR0L = BAUD_PRESCALE;
	UBRR0H = BAUD_PRESCALE >>8;
};
