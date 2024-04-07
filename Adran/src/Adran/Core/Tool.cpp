#include "AdranPreCompile.h"
#include "Tool.h"

namespace Adran
{
	std::mt19937 Random::s_randomEngine;

	std::uniform_int_distribution<std::mt19937::result_type> Random::s_distribution;
}