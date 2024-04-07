#pragma once
#include<glm/glm.hpp>
#include"Adran\Renderer\RenderObject\Buffer.h"

namespace Adran
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec3 Normal;
		glm::vec4 boneIds = glm::vec4(0.0f);
		glm::vec4 boneWeights = glm::vec4(0.0f);

		static inline BufferLayout GetLayout()
		{
			return {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
				{ "a_Normal", ShaderDataType::Float3 },
				{ "a_BoneIndex", ShaderDataType::Float4 },
				{ "a_BoneWeights", ShaderDataType::Float4 }
			};
		}
	};
}