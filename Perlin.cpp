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

void perlinNoise::calcBrownianFractal(float* arr, int xsize, int ysize,
	int startOctave, int endOctave, double amplitude, double frequency)
{
	int counter = 1;
	for (double k=startOctave; k<=endOctave; k*=frequency)
	{
		float invAmp = (float)1.0/(pow(amplitude,counter));
		for (int i=0; i<ysize; i++)
		{
			for (int j=0; j<xsize; j++)
			{
				arr[xsize*i+j] += invAmp*(float)this->getPt(((double)j/xsize)*(double)(k), ((double)i/ysize)*(double)(k));
			}
		}
		counter++;
	}

	float highest = 0.0, lowest = 0.0;

	for (int i=0; i<ysize; i++)
	{
		for (int j=0; j<xsize; j++)
		{
			highest = arr[xsize*i + j] > highest ? arr[xsize*i + j] : highest;
			lowest  = arr[xsize*i + j] < lowest ? arr[xsize*i + j] : lowest;
		}
	}

	float conversionFactor = (float)1.0/(fabs(highest) + fabs(lowest));	// to convert perlin noise to rgb format

	for (int i=0; i<ysize; i++)
	{
		for (int j=0; j<xsize; j++)
		{
			arr[xsize*i + j] += fabs(lowest);		// note, if lowest > 0, then this will cause the lowest value to be > 0, not 0
			arr[xsize*i + j] *= conversionFactor;
		}
	}
}

void perlinNoise::calcBrownianFractal(float* arr, int xsize, int ysize,
	int startOctave, int endOctave, double amplitude, double frequency,
	std::function<float (float)> modifier)
{
    int largerDim = (xsize>ysize ? xsize : ysize);
	int counter = 1;
	for (double k=startOctave; k<=endOctave; k*=frequency, counter++)
	{
		float invAmp = (float)1.0/(pow(amplitude,counter));
		for (int i=0; i<ysize; i++)
		{
			for (int j=0; j<xsize; j++)
			{
				arr[xsize*i+j] += invAmp*(float)this->getPt(
                    ((double)j/largerDim)*(double)(k),
                    ((double)i/largerDim)*(double)(k));
			}
		}
	}

	float highest = 0.0, lowest = 0.0;

	for (int i=0; i<ysize; i++)
	{
		for (int j=0; j<xsize; j++)
		{
			highest = arr[xsize*i + j] > highest ? arr[xsize*i + j] : highest;
			lowest  = arr[xsize*i + j] < lowest ? arr[xsize*i + j] : lowest;
		}
	}

    // to convert perlin noise to rgb format
	float conversionFactor = (float)1.0/(fabs(highest) + fabs(lowest));

	for (int i=0; i<ysize; i++)
	{
		for (int j=0; j<xsize; j++)
		{
			arr[xsize*i + j] += fabs(lowest);		// note, if lowest > 0, then this will cause the lowest value to be > 0, not 0
			arr[xsize*i + j] *= conversionFactor;
			arr[xsize*i + j] = modifier(arr[xsize*i + j]);
			//arr[xsize*i + j] = (float)((int)(arr[xsize*i + j]*100)-(int)(arr[xsize*i + j]*100)%10)/100.0;
		}
	}
}

void perlinNoise::calcBrownianFractal(float* arr, int xsize, int ysize,
	int startOctave, int endOctave, double amplitude, double frequency,
	std::function<float (float, float, float)> modifier)
{
	int counter = 1;
	for (double k=startOctave; k<=endOctave; k*=frequency)
	{
		float invAmp = (float)1.0/(pow(amplitude,counter));
		for (int i=0; i<ysize; i++)
		{
			for (int j=0; j<xsize; j++)
			{
				arr[xsize*i+j] += invAmp*(float)this->getPt(((double)j/xsize)*(double)(k), ((double)i/ysize)*(double)(k));
			}
		}
		counter++;
	}

	float highest = 0.0, lowest = 0.0;

	for (int i=0; i<ysize; i++)
	{
		for (int j=0; j<xsize; j++)
		{
			highest = arr[xsize*i + j] > highest ? arr[xsize*i + j] : highest;
			lowest  = arr[xsize*i + j] < lowest ? arr[xsize*i + j] : lowest;
		}
	}

	float conversionFactor = (float)1.0/(fabs(highest) + fabs(lowest));	// to convert perlin noise to rgb format

	for (int i=0; i<ysize; i++)
	{
		for (int j=0; j<xsize; j++)
		{
			arr[xsize*i + j] += fabs(lowest);		// note, if lowest > 0, then this will cause the lowest value to be > 0, not 0
			arr[xsize*i + j] *= conversionFactor;
			arr[xsize*i + j] = modifier(arr[xsize*i + j], (float)j/xsize, (float)i/ysize);
			//contourLines (float)((int)(arr[xsize*i + j]*100)-(int)(arr[xsize*i + j]*100)%10)/100.0;
		}
	}
}
