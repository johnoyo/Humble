#include "Random.h"

std::mt19937_64 HBL::Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937_64::result_type> HBL::Random::s_Distribution;
