#pragma once
#include<Adran.h>
#include<filesystem>
namespace Adran
{
	class ContentBroserPanel
	{
	public:
		ContentBroserPanel();
		void OnImGuiRender();
	private:
	 	Ref<Texture2D> m_folderIcon;
		Ref<Texture2D> m_descriptionIcon;

		std::filesystem::path m_currentDiretory;
		std::filesystem::path m_baseDiretory;
	};
}