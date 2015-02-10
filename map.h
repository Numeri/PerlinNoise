#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Perlin.h"

class map{
	//perlinNoise elevNoise;
public:
	map();
	map(int size);
	void resize(int size);
	void randomize();

	std::vector< std::vector<float> > data;
};

#endif 