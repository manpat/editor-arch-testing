#include "common.h"
#include <random>

thread_local std::random_device g_rand_engine {};

float randf(float min, float max) {
	thread_local std::uniform_real_distribution<float> s_dist {};
	using Params = std::uniform_real_distribution<float>::param_type;

	return s_dist(g_rand_engine, Params {min, max});
}

