#ifndef PERLIN_H
#define PERLIN_H

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "gradient.h"
#include <functional>

double interp(double value);

class perlinNoise
{
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
     * Takes ptr to float array, column-major (TODO:make row-major),
     * x size of array, y size, beginning octave of noise, ending octave,
     * the amplitude of each octave (2 produces 1, 0.5, 0.25, etc.),
     * and the frequency.
     * With std::function<float (float, float, float)> modifier, which takes
     * the point's value, x position [0,1], y position [0,1] in that order.
     */
    template <typename Func=std::function<float (float, float, float)>>
    void calcBrownianFractal(float* arr, int xsize, int ysize, int startOctave,
                             int endOctave, double amplitude, double frequency,
                             Func modifier =
                             [](float val, float x, float y){return val;})
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
                // note, if lowest > 0, then this will cause the lowest value to be > 0, not 0
                arr[xsize*i + j] += fabs(lowest);
                arr[xsize*i + j] *= conversionFactor;
                arr[xsize*i + j] = modifier(arr[xsize*i + j], (float)j/largerDim, (float)i/largerDim);
            }
        }
    }
};

#endif
