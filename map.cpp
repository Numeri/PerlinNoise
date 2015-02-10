#include "map.h"

map::map()
{
	this->resize(128);
}

map::map(int size)
{
	this->resize(size);
}

void map::resize(int size)
{
	data.resize(size);
	for( int i = 0; i < size; i++)
	{
		data[i].resize(size);
	}
}

void map::randomize()
{

}