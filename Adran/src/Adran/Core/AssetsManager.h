#pragma once

#include "Adran/Renderer/RenderObject/Texture.h"
namespace Adran
{
	class AssetsManager
	{
	public:
		Ref<Texture2D> GetTexture2D(const std::string& filePath);
 	private:
		bool IsTextureExist(const std::string& filePath);
		bool IsFileExist(const std::string& filePath);
	public:
		static AssetsManager* GetInstance()
		{
			return s_instance;
		}
	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_texturesMap;
	private:  // statics
		static AssetsManager* s_instance;
	};
}