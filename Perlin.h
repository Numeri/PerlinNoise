#ifndef PERLIN_H
#define PERLIN_H

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "gradient.h"
#include <functional>

double interp(double value);

class perlinNoise{
	gradient gradientTable[256];
	int permutationTable[256];
	int seed;
public:
	perlinNoise();
	perlinNoise(int initSeed);

	int setSeed(int seedValue);
	int generate();
	gradient getGradient(int x, int y);
	double getPt(double x, double y);

	/*calcBrownianFractal: Calculates the Brownian Fractal of the noise.
	 *Takes ptr to float array, column-major (TODO:make row-major),
	 *x size of array, y size, beginning octave of noise, ending octave,
	 *the amplitude of each octave (2 produces 1, 0.5, 0.25, etc.),
	 *and the frequency.
	 */
	void calcBrownianFractal(float* arr, int xsize, int ysize, int startOctave,
		int endOctave, double amplitude, double frequency);

	/*calcBrownianFractal: Calculates the Brownian Fractal of the noise.
	 *With std::function<float (float)> modifier, which takes the
	 *point's value.
	 */
	void calcBrownianFractal(float* arr, int xsize, int ysize, int startOctave,
		int endOctave, double amplitude, double frequency, 
		std::function<float (float)> modifier);

	/*calcBrownianFractal: Calculates the Brownian Fractal of the noise.
	 *With std::function<float (float, float, float)> modifier, which takes
	 *the point's value, x position [0,1], y position [0,1] in that order.
	 */
	void calcBrownianFractal(float* arr, int xsize, int ysize, int startOctave,
		int endOctave, double amplitude, double frequency, 
		std::function<float (float, float, float)> modifier);
};

#endif