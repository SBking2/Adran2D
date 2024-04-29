#include "SceneHierachyPanel.h"
#include <../imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <../imgui/imgui_internal.h>

#include<filesystem>

namespace Adran
{
	const std::filesystem::path g_AssetPath;

	SceneHierachyPanel::SceneHierachyPanel(const Ref<Scene>& scene)
	{
		m_scene = scene;
	}

	SceneHierachyPanel::~SceneHierachyPanel()
	{

	}

	void SceneHierachyPanel::SetScene(const Ref<Scene>& scene)
	{
		m_scene = scene;
		m_selectObject = Entity();
	}

	void SceneHierachyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierachy");
		
		auto& nc = m_scene->Reg().view<entt::entity>();
		for (auto& entity : nc)
		{
			DrawEntityNode({entity, m_scene.get()});
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selectObject = Entity();
		}

		//右键空白的地方
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_scene->CreateEntity();
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_selectObject)
		{
			DrawProperties(m_selectObject);

			
		}
		ImGui::End();
	}

	//privates
	void SceneHierachyPanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().name;

		ImGuiTreeNodeFlags flag = (m_selectObject == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flag |= ImGuiTreeNodeFlags_SpanAvailWidth;

		{
			ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flag, name.c_str());
			if (ImGui::IsItemClicked())
			{
				m_selectObject = entity;
			}
			bool entityDeleted = false;
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Empty Entity"))
				{
					entityDeleted = true;
				}

				ImGui::EndPopup();
			}

			if (entityDeleted)
			{
				if (m_selectObject == entity)
				{
					m_selectObject = Entity();
				}
				m_scene->DestroyEntity(entity);
			}

		}
		//TODO : children
	}

	void SceneHierachyPanel::DrawProperties(Entity entity)
	{
		if (entity.HasComponent<NameComponent>()) {
			auto& tag = entity.GetComponent<NameComponent>().name;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}

		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				float aspectRatio = (float)Application::GetInstance().GetWindow().GetWidth() / (float)Application::GetInstance().GetWindow().GetHeight();
				m_selectObject.AddComponent<CameraComponent>(aspectRatio);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite"))
			{
				m_selectObject.AddComponent<SpriteComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
		DrawComponent<TransformComponent>("Transform", entity, [&](TransformComponent& tc)
			{
				DrawVec2Control("Position", tc.position, 1.0f);
	
				DrawVec2Control("Scale", tc.scale, 1.0f);

				float rotation = glm::degrees(tc.rotation);
				//ImGui::DragFloat("Rotation", &rotation);
				DrawFloatControl("Rotation", rotation, 1.0f);
				tc.rotation = glm::radians(rotation);
			});
		DrawComponent<CameraComponent>("Camera", entity, [&](CameraComponent& cc)
			{
				ImGui::Checkbox("Primary", &cc.isPrimary);

				const char* cameraTypeString[] = { "Orthgraphic", "Perspective" };
				const char* currentTypeString = cameraTypeString[(int)cc.camera.GetSceneCameraType()];
				if (ImGui::BeginCombo("Type", currentTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSeleted = currentTypeString == cameraTypeString[i];
						if (ImGui::Selectable(cameraTypeString[i], isSeleted))
						{
							currentTypeString = cameraTypeString[i];
							cc.camera.SetType((SceneCamera::SceneCameraType)i);
						}

						if (isSeleted)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (cc.camera.GetSceneCameraType() == SceneCamera::SceneCameraType::orthgraphic)
				{
					float size = cc.camera.GetOrthSize();
					if (ImGui::DragFloat("Size", &size), 0.01f)
						cc.camera.SetOrthSize(size);

					float Near = cc.camera.GetOrthNear();
					if (ImGui::DragFloat("Near", &Near), 0.01f)
						cc.camera.SetOrthNear(Near);

					float Far = cc.camera.GetOrthFar();
					if (ImGui::DragFloat("Far", &Far), 0.01f)
						cc.camera.SetOrthFar(Far);
				}
				else
				{
					float fov = cc.camera.GetPerFOV();
					if (ImGui::DragFloat("FOV", &fov), 0.01f)
						cc.camera.SetPerFOV(fov);

					float Near = cc.camera.GetPerNear();
					if (ImGui::DragFloat("Near", &Near), 0.01f)
						cc.camera.SetPerNear(Near);

					float Far = cc.camera.GetPerFar();
					if (ImGui::DragFloat("Far", &Far), 0.01f)
						cc.camera.SetPerFar(Far);

				}
			});

		DrawComponent<SpriteComponent>("Sprite", entity, [&](SpriteComponent& sc)
			{
				if (sc.texture == nullptr)
				{
					ImGui::ImageButton((ImTextureID)1
						, { 150.0f, 150.0f }, { 0 ,1 }
					, { 1, 0 });
				}
				else
				{
					ImGui::ImageButton((ImTextureID)sc.texture->GetRenderID()
						, { 150.0f, 150.0f }, { 0 ,1 }
					, { 1, 0 });
				}
				

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						sc.texture = AssetsManager::GetInstance()->GetTexture2D(texturePath.string());
					}

					ImGui::EndDragDropTarget();
				}

				ImGui::ColorEdit4("Color", glm::value_ptr(sc.color));
			});
		
	}

	template<typename T, typename UIFunction>
	void SceneHierachyPanel::DrawComponent(const std::string& name, Entity entity, UIFunction function)
	{
		ImGuiTreeNodeFlags treeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap;
		treeNodeFlag |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4});
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::CollapsingHeader(name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}


			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				function(component);
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierachyPanel::DrawVec2Control(const std::string& label, glm::vec2& value, float resetValue, float columWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		//button样式设置
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.25f, 0.1, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.6f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize))
			value.x = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3, 1.0f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });

		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize))
			value.y = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	
	void SceneHierachyPanel::DrawFloatControl(const std::string& label, float& value, float resetValue, float columWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		//button样式设置
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.25f, 0.1, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.6f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize))
			value = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
}