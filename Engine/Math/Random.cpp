#include "Random.h"

#include <random>
#include <limits>

namespace nh
{
	std::default_random_engine g_rng;

	void SeedRandom(unsigned int seed) { g_rng.seed(seed); }

	float Random()
	{
		std::uniform_real_distribution<float> dist{ 0, 1 };
		return dist(g_rng);
	}

	float RandomRange(float min, float max)
	{
		std::uniform_real_distribution<float> dist{ min, max };
		return dist(g_rng);
	}

	int RandomInt()
	{
		std::uniform_int_distribution<int> dist{ 0, std::numeric_limits<int>::max() };
		return dist(g_rng);
	}

	int RandomInt(int max)
	{
		std::uniform_int_distribution<int> dist{ 0, max - 1 };
		return dist(g_rng);
	}

	int RandomRangeInt(int min, int max)
	{
		std::uniform_int_distribution<int> dist{ min, max };
		return dist(g_rng);
	}
}