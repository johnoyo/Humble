#include "Random.h"

std::mt19937 HBL::Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> HBL::Random::s_Distribution;
