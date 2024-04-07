#pragma once
#pragma once
#include<glm/glm.hpp>
#include"Adran\Renderer\RenderObject\Buffer.h"

namespace Adran
{
	struct SkinnedVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec3 Normal;

		static inline BufferLayout GetLayout()
		{
			return {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
				{ "a_Normal", ShaderDataType::Float3 }
			};
		}
	};
}