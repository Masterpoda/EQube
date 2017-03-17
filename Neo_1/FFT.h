#ifndef FFT_H
#define FFT_H

#include <math.h>
#include <limits.h>


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
unsigned int reverse(unsigned int number);
int populate(vector_2 z_vectors[], unsigned int size_N, float z[]);

//common matrix multiplications
vector_2 multiplyF2(vector_2 z2);
vector_2 multiplyD2(vector_2 triangle_2);
vector_4 multiplyD4(vector_4 triangle_2);

//populates vectors with permuted values
void populate_8(vector_2 *z1, vector_2 *z2, vector_2 *z3, vector_2 *z4, float z[]);

//complex/vector math
complex c_Add(complex c_1, complex c_2);
vector_2 v_Add2(vector_2 v1, vector_2 v2);
vector_4 v_Add4(vector_4 v1, vector_4 v2);
vector_2 v_Sub2(vector_2 v1, vector_2 v2);
vector_4 v_Sub4(vector_4 v1, vector_4 v2);
complex c_Sub(complex c_1, complex c_2);
complex c_Mult(complex c_1, complex c_2);

float eval_Func_8(X_k d_vector, float point);
X_k combine_final_8(vector_4 Square, vector_4 Diamond);


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

	complex d1;
	d1.real = 1.0;
	d1.imaginary = 0;

	complex d2;
	d2.real = 1.0/sqrt(2.0);
	d2.imaginary = -1.0/sqrt(2.0);

	complex d3;
	d3.real = 0;
	d3.imaginary = -1.0;

	complex d4;
	d4.real = -1.0/sqrt(2.0);
	d4.imaginary = -1.0/sqrt(2.0);

	//d1 - d4 values verified


	diamond.v[0] = c_Mult(triangle_2.v[0], d1);
	diamond.v[1] = c_Mult(triangle_2.v[1], d2);
	diamond.v[2] = c_Mult(triangle_2.v[2], d3);
	diamond.v[3] = c_Mult(triangle_2.v[3], d4);
	
	return diamond;
}

//function used to simultaneously permute vector and 
void populate_8(vector_2 *z1, vector_2 *z2, vector_2 *z3, vector_2 *z4, float z[])
{
	// Setting real Part	// Setting imaginary part
	//z1
	z1->v[0].real = z[0]; z1->v[0].imaginary = 0;
	z1->v[1].real = z[4]; z1->v[1].imaginary = 0;

	//z2
	z2->v[0].real = z[2]; z2->v[0].imaginary = 0;
	z2->v[1].real = z[6]; z2->v[1].imaginary = 0;

	//z3
	z3->v[0].real = z[1]; z3->v[0].imaginary = 0;
	z3->v[1].real = z[5]; z3->v[1].imaginary = 0;

	//z4
	z4->v[0].real = z[3]; z4->v[0].imaginary = 0;
	z4->v[1].real = z[7]; z4->v[1].imaginary = 0;

}

int populate(vector_2 z_vectors[], unsigned int size_N, float z[])//max size is UINT_MAX
{
	if(size_N % 2 != 0)
	{
		return 1; //return 1 if the array isnt a multiple of 2
	}
	int bitsize = log(size_N)/log(2); //how many bits to describe the vector

	unsigned int i, j, index, shiftAmount = (sizeof(unsigned int)*8) - bitsize;
	for(i = 0; i < size_N/2; i++)
	{
		for (j = 0; j < 2; j++)
		{
			index = reverse(i + j) >> shiftAmount; //which Z value goes in this
			z_vectors[i].v[i+j].real = z[index];///size_N; //unitizing in place.
			z_vectors[i].v[i+j].imaginary = 0;
		}
	}
	return 0;
}

unsigned int reverse(unsigned int number)
{
	unsigned int r = number; // r will be reversed bits of v; first get LSB of v
	unsigned int s = sizeof(number) * CHAR_BIT - 1; // extra shift needed at end

	for (number >>= 1; number; number >>= 1)
	{
		r <<= 1;
		r |= number & 1;
		s--;
	}
	r <<= s; // shift when v's highest bits are zero

	return r;
}

vector_4 combine_two(vector_2 Square, vector_2 Diamond)
{
	vector_4 step_2;
	vector_2 temp, temp2; //storing sum and difference in advance

	temp = v_Add2(Diamond, Square);
	temp2 = v_Sub2(Square, Diamond);

	step_2.v[0] = temp.v[0];
	step_2.v[1] = temp.v[1];
	step_2.v[2] = temp2.v[0];
	step_2.v[3] = temp2.v[1];

	return step_2;
}

X_k combine_final(vector_4 Square, vector_4 Diamond)
{
	X_k transformed;
	vector_4 temp, temp2;//storing sum and difference in advance

	temp = v_Add4(Square, Diamond);
	temp2 = v_Sub4(Square, Diamond);

	transformed.d[0] = temp.v[0];
	transformed.d[1] = temp.v[1];
	transformed.d[2] = temp.v[2];
	transformed.d[3] = temp.v[3];
	transformed.d[4] = temp2.v[0];
	transformed.d[5] = temp2.v[1];
	transformed.d[6] = temp2.v[2];
	transformed.d[7] = temp2.v[3];

	
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

//add vectors, returns the result
vector_2 v_Add2(vector_2 v1, vector_2 v2)
{
	vector_2 temp;

	temp.v[0] = c_Add(v1.v[0], v2.v[0]);
	temp.v[1] = c_Add(v1.v[1], v2.v[1]);

	return temp;
}

vector_4 v_Add4(vector_4 v1, vector_4 v2)
{
	vector_4 temp;

	temp.v[0] = c_Add(v1.v[0], v2.v[0]);
	temp.v[1] = c_Add(v1.v[1], v2.v[1]);
	temp.v[2] = c_Add(v1.v[2], v2.v[2]);
	temp.v[3] = c_Add(v1.v[3], v2.v[3]);

	return temp;

}

complex c_Sub(complex c_1, complex c_2)
{
	complex temp;
	temp.real = c_1.real - c_2.real;
	temp.imaginary = c_1.imaginary - c_2.imaginary;
	
	return temp;
	
}

vector_2 v_Sub2(vector_2 v1, vector_2 v2)
{
	vector_2 temp;

	temp.v[0] = c_Sub(v1.v[0], v2.v[0]);
	temp.v[1] = c_Sub(v1.v[1], v2.v[1]);

	return temp;
}

vector_4 v_Sub4(vector_4 v1, vector_4 v2)
{
	vector_4 temp;

	temp.v[0] = c_Sub(v1.v[0], v2.v[0]);
	temp.v[1] = c_Sub(v1.v[1], v2.v[1]);
	temp.v[2] = c_Sub(v1.v[2], v2.v[2]);
	temp.v[3] = c_Sub(v1.v[3], v2.v[3]);

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


float eval_Func_8(X_k d_vector, float point)
{
	float final_value = d_vector.d[0].real / 2; //getting constant value
	
	float Im, Re;

	int i = 0;
	//start at 1 since we already evaluated k=0
	for(i = 1; i < 8; i++)
	{
		Re = d_vector.d[i].real;		//assigning value to a variable means fewer structure accesses
		Im = d_vector.d[i].imaginary;	//FFT will be a little faster

		if(Re > 0.001 || Re < -0.001)//cosine part
			final_value += Re*cos(i*point);

		if(Im > 0.001 || Im < -0.001)//sine part
			final_value += Im*sin(i*point);
	}
	return final_value;
}

//gets an X_k vector from an array of y values (assuming size is 8)
X_k evalX_k(float y[])
{
	X_k transformed;
	Unitize(y);//unitizing values of y

	vector_2 z1, z2, z3, z4; //vectors to hold permuted values

	vector_4 z1_2, z2_2; //vectors holding second stage of values

	vector_2 diamond_z2, diamond_z4;

	vector_4 diamond_4;

	populate_8(&z1, &z2, &z3, &z4, y); 


	//Muliply all Z vectors by F2 (the whole reason the FFT exists is so we can do this instead of higher N values for FN)
	z1 = multiplyF2(z1);
	z2 = multiplyF2(z2);
	z3 = multiplyF2(z3);
	z4 = multiplyF2(z4);

	diamond_z2 = multiplyD2(z2);//obtaining 'Diamond' vectors
	diamond_z4 = multiplyD2(z4);

	z1_2 = combine_two(z1, diamond_z2);
	z2_2 = combine_two(z3, diamond_z4);

	//verified up to here

	diamond_4 = multiplyD4(z2_2);

	transformed = combine_final(z1_2, diamond_4);

	return transformed;
}

#endif