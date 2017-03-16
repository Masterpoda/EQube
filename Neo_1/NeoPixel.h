#define PIXELS 24 // Number of pixels in the string

// These values depend on which pin your string is connected to and what board you are using
// More info on how to find these at http://www.arduino.cc/en/Reference/PortManipulation

// These values are for digital pin 8 on an Arduino Yun or digital pin 12 on a DueMilinove
// Note that you could also include the DigitalWriteFast header file to not need to to this lookup.

#define PIXEL_PORT PORTB // Port of the pin the pixels are connected to
#define PIXEL_DDR DDRB // Port of the pin the pixels are connected to
#define PIXEL_BIT 4 // Bit of the pin the pixels are connected to

// These are the timing constraints taken mostly from the WS2812 datasheets
// These are chosen to be conservative and avoid problems rather than for maximum throughput

#define T1H  900    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns

#define F_CPU 16000000L

#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns

#define RES 8000    // Width of the low gap between bits to cause a frame to latch

// Here are some convenience defines for using nanoseconds specs to generate actual CPU delays

#define NS_PER_SEC (1000000000L) // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives

#define CYCLES_PER_SEC (F_CPU)

#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )

#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )

//#define DELAY_CYCLES(n) ( ((n)&gt;0) ? __builtin_avr_delay_cycles( n ) : __builtin_avr_delay_cycles( 0 ) ) // Make sure we never have a delay less than zero

// Actually send a bit to the string. We turn off optimizations to make sure the compile does
// not reorder things and make it so the delay happens in the wrong place.


#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "FFT.h"


typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}pixel_struct;

pixel_struct STRAND[PIXELS];

inline void sendBit( int bitVal )
{
	if(bitVal)
	{
		asm volatile(
		"sbi %[port], %[bit] \n\t"				// Set the output bit
		".rept %[onCycles] \n\t"                                // Execute NOPs to delay exactly the specified number of cycles
		"nop \n\t"
		".endr \n\t"
		"cbi %[port], %[bit] \n\t"                              // Clear the output bit
		".rept %[offCycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
		"nop \n\t"
		".endr \n\t"
		::
		[port]		"I" (_SFR_IO_ADDR(PIXEL_PORT)),
		[bit]		"I" (PIXEL_BIT),
		[onCycles]	"I" (NS_TO_CYCLES(T1H) - 2),		// 1-bit width less overhead  for the actual bit setting, note that this delay could be longer and everything would still work
		[offCycles] 	"I" (NS_TO_CYCLES(T1L) - 2)			// Minimum interbit delay. Note that we probably don't need this at all since the loop overhead will be enough, but here for correctness

		
		);
	}
	else
	{
		asm volatile (
		"sbi %[port], %[bit] \n\t"				// Set the output bit
		".rept %[onCycles] \n\t"				// Now timing actually matters. The 0-bit must be long enough to be detected but not too long or it will be a 1-bit
		"nop \n\t"                                              // Execute NOPs to delay exactly the specified number of cycles
		".endr \n\t"
		"cbi %[port], %[bit] \n\t"                              // Clear the output bit
		".rept %[offCycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
		"nop \n\t"
		".endr \n\t"
		::
		[port]		"I" (_SFR_IO_ADDR(PIXEL_PORT)),
		[bit]		"I" (PIXEL_BIT),
		[onCycles]	"I" (NS_TO_CYCLES(T0H) - 2),
		[offCycles]	"I" (NS_TO_CYCLES(T0L) - 2)

		);

	}

}

inline void sendByte( unsigned char byte ) {
	
	for( unsigned char bit = 0 ; bit < 8 ; bit++ ) {
		
		sendBit( byte & 0b10000000);	// Neopixel wants bit in highest-to-lowest order
		byte <<= 1;	 					// so send highest bit (bit #7 in an 8-bit byte since they start at 0)
										// and then shift left so bit 6 moves into 7, 5 moves into 6, etc
	}
}

void ledsetup() {

	PIXEL_DDR = PIXEL_DDR | (int)pow(PIXEL_BIT, 2); //setting signal output LED
	
}

inline void sendPixel( unsigned char r, unsigned char g , unsigned char b )  {

	sendByte(g);          // Neopixel wants colors in green then red then blue order
	sendByte(r);
	sendByte(b);

}


void show() 
{
	_delay_us( (RES / 1000UL) + 1);				// Round up since the delay must be _at_least_ this long (too short might not work, too long not a problem)
}


void showColor( unsigned char r , unsigned char g , unsigned char b ) {
	
	cli();
	for( int p=0; p<PIXELS; p++ ) {
		sendPixel( r , g , b );
	}
	sei();
	show();
	
}

inline void sendPixelStruct( pixel_struct pixel )  {

	sendByte(pixel.g);          // Neopixel wants colors in green then red then blue order
	sendByte(pixel.r);
	sendByte(pixel.b);

}

void showColorStruct( pixel_struct pixel ) {
	
	cli();
	for( int p=0; p<PIXELS; p++ ) {
		sendPixelStruct( pixel );
	}
	sei();
	show();
	
}

void displayStrand()
{
	cli();
	for( int p=0; p<PIXELS; p++ ) {
		sendPixelStruct( STRAND[p] );
	}
	sei();
	show();
}

void setStrand(pixel_struct pixel)
{
	int i = 0;
	for(i = 0; i < PIXELS; i++)
	{
		STRAND[i].r = pixel.r;
		STRAND[i].g = pixel.g;
		STRAND[i].b = pixel.b;
	}

}

void colorwipe()
{
	int i = 0;
	pixel_struct next;

	next.b = 0;
	next.r = 0;
	next.g = 0;
	setStrand(next);

	next.b = 100;
	for(i = 0; i < PIXELS; i++)
	{
		STRAND[i] = next;
		displayStrand();
		_delay_ms(100);
	}


	next.g = 0;
	next.b = 17;
	next.r = 44;
	for(i = 0; i < PIXELS; i++)
	{
		STRAND[i] = next;
		displayStrand();
		_delay_ms(100);
	}

	next.b = 0;
	next.g = 0;
	next.r = 100;
	for(i = 0; i < PIXELS; i++)
	{
		STRAND[i] = next;
		displayStrand();
		_delay_ms(100);
	}

	next.b = 0;
	next.r = 100;
	next.g = 100;
	for(i = 0; i < PIXELS; i++)
	{
		STRAND[i] = next;
		displayStrand();
		_delay_ms(100);
	}

	next.b = 0;
	next.r = 0;
	next.g = 100;
	for(i = 0; i < PIXELS; i++)
	{
		STRAND[i] = next;
		displayStrand();
		_delay_ms(100);
	}

	next.b = 100;
	next.g = 100;
	next.r = 0;
	
	for(i = 0; i < PIXELS; i++)
	{
		STRAND[i] = next;
		displayStrand();
		_delay_ms(100);
	}


}

void Map7Linear( int array[]) //adjust strands brightness according to 7 element array using linear interpolation.7
{
	STRAND[0].r = array[0];
	STRAND[4].r = array[1];
	STRAND[8].r = array[2];
	STRAND[12].r = array[3];
	STRAND[15].r = array[4];
	STRAND[19].r = array[5];
	STRAND[23].r = array[6];

	
}
