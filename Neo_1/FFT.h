#ifndef FFT_H
#define FFT_H

#include <math.h>


//Fast Fourier Transformation Functions
//This will not initially be for general purpose FFT, but rather FFT on an array of 8 values 
//(this is all we need since MSGEQ7 gives 7 values)
//We will zero pad at the first value

//Size of sample vector
#define SIZE 8 

/*********** Structure Definitions *************/

//complex number structure
typedef struct{
	float real;
	float imaginary;
}complex;

//a vector with 2 elements
typedef struct{
	complex v[2];
	
}vector_2;

//a vector with 4 elements
typedef struct{
	complex v[4];
	
}vector_4;

//final transformed vector
typedef struct{
	complex d[8];
	
}X_k;


/*********** Function Declarations *************/
void Unitize(float y[]);
vector_2 multiplyF2(vector_2 z2);
vector_2 multiplyD2(vector_2 triangle_2);
vector_4 multiplyD4(vector_4 triangle_2);
complex c_Add(complex c_1, complex c_2);
complex c_Sub(complex c_1, complex c_2);
complex c_Mult(complex c_1, complex c_2);
float eval_Func(X_k d_vector, float point);
X_k combine_final(vector_4 Square, vector_4 Diamond);

/*********** Function Definitions *************/

//incoming array must be size 8, zero padded
void Unitize(float y[])
{
	int i = 0;
	for(i = 0; i < SIZE; i++)
	{
		y[i] /= SIZE;
	}
}

//takes in vector and returns result of F2 multiplication
//note that:
/*
	F2 = [1 , 1]
		 [1 ,-1]
*/
vector_2 multiplyF2(vector_2 z2)
{
	complex temp;
	temp = c_Add(z2.v[0], z2.v[1]);
	z2.v[1] = c_Sub(z2.v[0], z2.v[1]);
	z2.v[0] = temp;
	return z2;
}

/*
 * D vector exists in the permutative reduction of the fourier matrix
 * it is defined as a diagonal matrix with entries 1, omega, omega^2... omega^n-1

 * where omega = e^(pi/N), N = size of matrix
*/

vector_2 multiplyD2(vector_2 triangle_2)
{
	vector_2 diamond;
	complex d2;

	d2.real = 0;
	d2.imaginary = -1;

	diamond.v[0] = triangle_2.v[0];
	diamond.v[1] = c_Mult(d2, triangle_2.v[1]);
	
	return diamond;
}

vector_4 multiplyD4(vector_4 triangle_2)
{
	vector_4 diamond;

	complex d2;
	d2.real = 1.0/sqrt(2.0);
	d2.imaginary = -1.0/sqrt(2.0);

	complex d3;
	d2.real = 0;
	d2.imaginary = -1.0;

	complex d4;
	d2.real = -1.0/sqrt(2.0);
	d2.imaginary = -1.0/sqrt(2.0);

	diamond.v[0] = triangle_2.v[0];
	diamond.v[1] = c_Mult(triangle_2.v[1], d2);
	diamond.v[2] = c_Mult(triangle_2.v[2], d3);
	diamond.v[3] = c_Mult(triangle_2.v[3], d4);
	
	return diamond;
}

X_k combine_final(vector_4 Square, vector_4 Diamond)
{
	X_k transformed;
	
	return transformed;
}

//function adds two complex numbers and returns the result
complex c_Add(complex c_1, complex c_2)
{
	complex temp;
	temp.real = c_1.real + c_2.real;
	temp.imaginary = c_1.imaginary + c_2.imaginary;
	
	return temp;
}

complex c_Sub(complex c_1, complex c_2)
{
	complex temp;
	temp.real = c_1.real - c_2.real;
	temp.imaginary = c_1.imaginary - c_2.imaginary;
	
	return temp;
	
}

//function multiplies two complex numbers and returns the result
complex c_Mult(complex c_1, complex c_2)
{
	complex temp;
	temp.real = (c_1.real*c_2.real) - (c_1.imaginary*c_2.imaginary);
	temp.imaginary = (c_1.real*c_2.imaginary) + (c_1.imaginary*c_2.real);
	
	return temp;
	
}


float eval_Func(X_k d_vector, float point)
{
	float final_value = d_vector.d[0].real / 2; //getting constant value
	
	int i = 0;
	//start at 1 since we already evaluated k=0
	for(i = 1; i < 8; i++)
	{
		if(d_vector.d[i].real > 0.001)//cosine part
			final_value += d_vector.d[i].real*cos(i*point);

		if(d_vector.d[i].imaginary > 0.001)//sine part
			final_value += d_vector.d[i].imaginary*sin(i*point);
	}
	return final_value;
}

#endif