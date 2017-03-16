
#include "NeoPixel.h"
#include <stdlib.h>

int main()
{
	pixel_struct pixel;

	pixel.r = 10;
	pixel.g = 0;
	pixel.b = 0;

	ledsetup();
	int seven[7] = {1,5,3,2,6,0,4};
	setStrand(pixel);
	Map7Linear(seven);
	while (1)
	{
		//colorwipe();

		int k = 0;
		for(k = 0; k < 7; k++)
		{
			seven[k] = rand()%10;
		}
		Map7Linear(seven);
		_delay_ms(500);
		displayStrand();

	}

	return 0;
}

