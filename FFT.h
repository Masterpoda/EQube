#ifndef FFT_H
#define FFT_H

#include <math.h>


//Fast Fourier Transformation Functions
//This will not initially be for general purpose FFT, but rather FFT on an array of 8 values 
//(this is all we need since MSGEQ7 gives 7 values)
//We will zero pad at the first value

//Size of sample vector
#define SIZE 8 

//complex number structure
typedef struct {
	float real;
	float imaginary;
}complex;


typedef struct{
	complex S[2];
	
}Square_2;

typedef struct{
	complex S[4];
	
}Square_4;

typedef struct{
	complex S[8];
	
}X_k;




//incoming array must be size 8, zero padded
void Unitize(float y)
{
	int i = 0;
	for(i = 0; i < SIZE; i++)
	{
		y[i] /= SIZE;
	}
}

//takes in two vector values and stores them in place
//note that:
/*
	F2 = [1 , 1]
		 [1 ,-1]
*/
void multiplyF2(float &i0, float &i1)
{
	float temp = i0 + i1;
	i1 = i0 - i1;
	i0 = temp;
}

Square_2 multiplyD2(Square_2 triangle_2)
{
	Square_2 diamond;
	
	return diamond;
}

Square_4 multiplyD4(Square_4 triangle_2)
{
	Square_4 diamond;
	
	return diamond;
}

X_k combine_final(Square_4 Square, Square_4 Diamond)
{
	X_k transformed;
	
	return transformed;
}

complex c_Add(complex c_1, complex c_2)
{
	complex temp;
	temp.real = c_1.real + c_2.real;
	temp.imaginary = c_1.imaginary + c_2.imaginary;
	
	return temp;
	
}

complex c_Mult(complex c_1, complex c_2)
{
	complex temp;
	temp.real = (c_1.real*c_2.real) - (c_1.imaginary*c_2.imaginary);
	temp.imaginary = (c_1.real*c_2.imaginary) + (c_1.imaginary*c_2.real);
	
	return temp;
	
}

float eval_X_k(X_k d_vector, float point)
{
	float final_value = d_vector.d0.real / 2; //getting constant value
	
	int i = 0;
	for(i = 0; i < 8; i++)
	{
		if(
	}
}

#endif FFT_H