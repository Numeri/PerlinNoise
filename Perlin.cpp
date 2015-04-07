#include "Perlin.h"


double interp(double value)
{
	return 6.0*pow(value, 5.0) - 15.0*pow(value, 4.0) + 10.0*pow(value, 3.0);
}

perlinNoise::perlinNoise() : seed((int)time(NULL))
{}

perlinNoise::perlinNoise(int initSeed) : seed(initSeed)
{}

int perlinNoise::setSeed(int seedValue)
{
	seed = seedValue;
	return 0;
}

int perlinNoise::generate()
{
	srand(seed);

	for (int i=0; i<256; i++)
	{
		permutationTable[i] = -1;
	}

	int j = rand() & 255;

	for (int i=0; i<256; i++)
	{
		while ( permutationTable[j] != -1)
			j = rand() & 255;	// j = rand() mod 256

		permutationTable[j] = i;
		gradientTable[i].x = (double)(rand() & 255) - 128.0; // rand() mod 256
		gradientTable[i].y = (double)(rand() & 255) - 128.0; //   "     "   "

		double gradLength = sqrt(gradientTable[i].x*gradientTable[i].x + gradientTable[i].y*gradientTable[i].y);
		gradientTable[i] = gradientTable[i]/gradLength;
	}

	return 0;
}

gradient perlinNoise::getGradient(int x, int y)
{
	return gradientTable[ permutationTable[ ( x + permutationTable[(y + 256) % 256] + 256 ) % 256 ] + 2 ];
}

double perlinNoise::getPt(double x, double y)
{
	int x0 = (int)(x);
	int y0 = (int)(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	/*if ( x == floor(x) )
		x1 = x0;
	if ( y == floor(y) )
		y1 = y0;*/

	gradient x0y0 = getGradient(x0, y0);	//
	gradient x1y1 = getGradient(x1, y1);	// gradients of the integer points
	gradient x0y1 = getGradient(x0, y1);	// surrounding the point requested.
	gradient x1y0 = getGradient(x1, y0);	//

	gradient from00, from11, from01, from10; // vectors from integer points to point

	from00.x = x-x0; from00.y = y-y0;
	from11.x = x-x1; from11.y = y-y1;
	from01.x = x-x0; from01.y = y-y1;
	from10.x = x-x1; from10.y = y-y0;

	double dot00 = dotProduct(x0y0, from00);
	double dot11 = dotProduct(x1y1, from11);
	double dot01 = dotProduct(x0y1, from01);
	double dot10 = dotProduct(x1y0, from10);

	dot00 = (1-interp(x-x0))*dot00 + interp(x-x0)*dot10;	// reusing these doubles to store temp
	dot11 = (1-interp(x-x0))*dot01 + interp(x-x0)*dot11;	// values.

	return (1-interp(y-y0))*dot00 + interp(y-y0)*dot11;
}
