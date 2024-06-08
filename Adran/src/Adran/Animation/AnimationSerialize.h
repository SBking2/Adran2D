#pragma once

#include "Animation.h"
#include "Adran/Core/Core.h"

#include <string>
#include"yaml-cpp\yaml.h"

namespace Adran
{
	class AnimationSerialize
	{
	public:
		static void SerializeAnimation(const Ref<Animation>& animation, const std::string& path);
		static Ref<Animation> DeSerializeAnimation(const std::string& path);
	};
}