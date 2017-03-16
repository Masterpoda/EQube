
#include "NeoPixel.h"
#include <stdlib.h>

int main()
{
	pixel_struct pixel;

	pixel.r = 10;
	pixel.g = 0;
	pixel.b = 0;

	

	ledsetup();
	int seven[8] = {0,1,3,5,1,8,1,5};//technically 8
	float seven_F[8] = {0,20,30,50,10,80,20,50};//technically 8
	setStrand(pixel);
	Map7FFT(seven_F);
	
	displayStrand();
	
	unsigned int counter; //used for sinusoudal light pulsing
	while (1)
	{
		//colorwipe();

		int k = 0;
		int temp;
		for(k = 0; k < 7; k++)
		{
			temp = rand()%40 + 10;
			seven_F[k] = (float)temp;
			seven[k] = temp;
		}

		seven_F[7] = (seven_F[0] + seven_F[6])/2; //interpolating an 8th position since the MSGEQ7 only gives 7
		seven[7] = (seven[0] + seven[6])/2;
		
		_delay_ms(250);

		Map7FFT(seven_F);
		//Map7Linear(seven);

		displayStrand();
		counter++;
	}

	return 0;
}

