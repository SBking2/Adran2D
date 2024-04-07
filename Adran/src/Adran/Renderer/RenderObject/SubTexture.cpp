#include"AdranPreCompile.h"
#include"SubTexture.h"

namespace Adran
{
	SubTexture::SubTexture(const Ref<Texture2D>& texture, const glm::vec2& maxCoords, const glm::vec2& minCoords)
	{
		m_texture = texture;
		m_coords = new glm::vec2[4];
		m_coords[0] = minCoords;
		m_coords[1] = { maxCoords.x, minCoords.y };
		m_coords[2] = maxCoords;
		m_coords[3] = { minCoords.x, maxCoords.y };
	}
	Ref<Texture2D> SubTexture::GetTexture()
	{
		return m_texture;
	}
	glm::vec2* SubTexture::GetCoords()
	{
		return m_coords;
	}
	Ref<SubTexture> SubTexture::CreateSubTexture(const Ref<Texture2D>&texture, const glm::vec2 & cellSize, const glm::vec2 & textureSize
		, uint32_t* location)
	{
		glm::vec2 maxCoord = { ((location[2] - 1) * cellSize.x) / textureSize.x, ((location[3] - 1) * cellSize.y) / textureSize.y };
		glm::vec2 minCoord = { ((location[0] - 1) * cellSize.x) / textureSize.x, ((location[1] - 1) * cellSize.y) / textureSize.y };

		return CreateRef<SubTexture>(texture, maxCoord, minCoord);
	}
}