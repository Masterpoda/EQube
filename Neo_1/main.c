
#include "NeoPixel.h"
#include <stdlib.h>

int main()
{
	pixel_struct pixel;

	pixel.r = 10;
	pixel.g = 0;
	pixel.b = 0;

	

	ledsetup();
	int seven[8] = {0,0,80,0,0,80,0,0};//technically 8
	float seven_F[8] = {0,0,0,0,255,0,0,0};//technically 8
	setStrand(pixel);
	Map7FFT(seven_F);

	unsigned int test = 0x05;

	

	seven[0] = reverse(test);
	//Map7Linear(seven);
	
	vector_2 zs[4];

	float z[8] = {0,1,2,3,4,5,6,7};
	unsigned int Size = 8;
	populate(zs, Size, z);
	Fourierize(zs, 8);
	multiplyDN_All(zs,8,2);
	
	//multiplyDN(&zs[1], 2);
	
	//multiplyDN(&zs[3], 2);
	
	Combine_SD(zs,8,2);

	multiplyDN_All(zs,8,4);//problem

	Combine_SD(zs,8,4);

	unsigned int k = 1;
	k = reverse(k);
	z[0] = k;
	displayStrand();
	
	unsigned int counter; //used for sinusoudal light pulsing
	while (1)
	{
		//colorwipe();

		int k = 0;
		int temp;
		
		for(k = 0; k < 7; k++)
		{
			temp = rand()%50 + 0;
			seven_F[k] = (float)temp;
			seven[k] = temp;
		}
		

		seven_F[7] = (seven_F[0] + seven_F[6])/2; //interpolating an 8th position since the MSGEQ7 only gives 7
		seven[7] = (seven[0] + seven[6])/2;
		
		_delay_ms(250);

		//Map7FFT(seven_F);
		Map7Linear(seven);

		displayStrand();
		counter++;
	}

	return 0;
}

