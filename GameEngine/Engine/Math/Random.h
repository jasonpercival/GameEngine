#ifndef RANDOM_H
#define RANDOM_H

#include <chrono>
#include <cstdlib>

class Random
{
public:
	static void SetSeed()
	{
		srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	}

	static float Range(float min_, float max_)
	{
		float random = (((float)rand()) / (float)RAND_MAX) * (max_ - min_);
		return min_ + random;
	}
private:
	Random();
};

#endif
