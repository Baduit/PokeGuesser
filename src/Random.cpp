#include <random>

int get_random_number(int min_included, int max_included)
{
	static std::random_device r;
	static std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(min_included, max_included);
	return uniform_dist(e1);
}