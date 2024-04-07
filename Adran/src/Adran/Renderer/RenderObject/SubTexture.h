#pragma once
#include"Adran/Core/Core.h"
#include "Texture.h"
#include <glm/glm.hpp>
namespace Adran
{
	class SubTexture
	{
	public:
		SubTexture(const Ref<Texture2D>& texture,const glm::vec2& maxCoords, const glm::vec2& minCoords);
		Ref<Texture2D> GetTexture();
		glm::vec2* GetCoords();
		static Ref<SubTexture> CreateSubTexture(const Ref<Texture2D>& texture, const glm::vec2& cellSize, const glm::vec2& textureSize
			, uint32_t* location);
	private:
		Ref<Texture2D> m_texture;
		glm::vec2* m_coords;
	};
}