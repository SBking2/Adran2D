#include"AdranPreCompile.h"
#include"UUID.h"

#include <random>
#include <unordered_map>

namespace Adran
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	//hash �൱�ڴ洢����UUID
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine))
	{
	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}
}