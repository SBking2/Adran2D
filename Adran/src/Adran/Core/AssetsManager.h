#pragma once

#include "Adran/Renderer/RenderObject/Texture.h"
#include "Adran/Renderer/RenderObject/Model/Model.h"
namespace Adran
{
	class AssetsManager
	{
	public:
		Ref<Texture2D> GetTexture2D(const std::string& filePath);
		Ref<Model> GetModel(const std::string& filePath);
 	private:
		bool IsTextureExist(const std::string& filePath);
		bool IsModelExist(const std::string& filePath);
		bool IsFileExist(const std::string& filePath);
	public:
		static AssetsManager* GetInstance()
		{
			return s_instance;
		}
	private:
		std::unordered_map<std::string, Ref<Model>> m_modelMap;
		std::unordered_map<std::string, Ref<Texture2D>> m_texturesMap;
	private:  // statics
		static AssetsManager* s_instance;
	};
}