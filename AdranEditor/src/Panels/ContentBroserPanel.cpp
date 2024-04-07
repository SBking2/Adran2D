#include "ContentBroserPanel.h"

#include<imgui/imgui.h>

namespace Adran
{
	const std::filesystem::path g_AssetPath = "assets";

	ContentBroserPanel::ContentBroserPanel()
	{
		m_baseDiretory = g_AssetPath;
		m_currentDiretory = m_baseDiretory;

		m_folderIcon = AssetsManager::GetInstance()->GetTexture2D("assets/Icon/folder.png");
		m_descriptionIcon = AssetsManager::GetInstance()->GetTexture2D("assets/Icon/description.png");
	}

	void ContentBroserPanel::OnImGuiRender()
	{
		ImGui::Begin("Directory");

		if (m_currentDiretory != std::filesystem::path(m_baseDiretory))
		{
			if (ImGui::Button("<-"))
			{
				m_currentDiretory = m_currentDiretory.parent_path();
			}
		}

		static float padding = 10.0f;
		static float thumbnailSize = 100.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
		for (auto& file : std::filesystem::directory_iterator(m_currentDiretory))
		{
			const auto& path = file.path();
			std::string fileNameString = path.filename().string();

			ImGui::PushID(fileNameString.c_str());

			Ref<Texture2D> icon = file.is_directory() ? m_folderIcon : m_descriptionIcon;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRenderID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			if(ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::SetDragDropPayload("ASSET", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			//双击进入目录
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (file.is_directory())
				{
					m_currentDiretory /= path.filename();
				}
					
			}

			//在图片下方显示文件名字
			ImGui::TextWrapped(fileNameString.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1);

		ImGui::End();
	}
}