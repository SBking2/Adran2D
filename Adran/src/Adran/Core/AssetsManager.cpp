#include "AdranPreCompile.h"
#include "AssetsManager.h"

namespace Adran
{
	AssetsManager* AssetsManager::s_instance = new AssetsManager();

	Ref<Texture2D> AssetsManager::GetTexture2D(const std::string& filePath)
	{
		if (!IsFileExist(filePath))
			return nullptr;

		if (IsTextureExist(filePath))
		{
			return m_texturesMap[filePath];
		}

		return m_texturesMap[filePath] = Texture2D::Create(filePath);
	}

	
	bool AssetsManager::IsTextureExist(const std::string& filePath)
	{
		if (m_texturesMap.find(filePath) != m_texturesMap.end())
		{
			return true;
		}
		return false;
	}

	bool AssetsManager::IsFileExist(const std::string& filePath)
	{
		std::ifstream file(filePath);
		return file.good();
	}

}