
#include "NeoPixel.h"
#include "FFT.h"

int main()
{
	ledsetup();
	// use SBI/CBI
	unsigned char r = 0, g = 0, b = 0;
	while (1)
	{
		r = 0; g = 0; b = 0;
		for(r = 0; r < 255; r++)
		{
			showColor(r,g,b);
			_delay_ms(1);
		}
		r = 0; g = 0; b = 0;
		for(g = 0; g < 255; g++)
		{
			showColor(r,g,b);
			_delay_ms(1);
		}
		r = 0; g = 0; b = 0;
		for(b = 0; b < 255; b++)
		{
			showColor(r,g,b);
			_delay_ms(1);
		}
	}

	return 0;
}

