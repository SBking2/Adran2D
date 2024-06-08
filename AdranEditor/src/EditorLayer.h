#pragma once
#include <Adran.h>
#include"ParticleSystem.h"
#include "Panels\SceneHierachyPanel.h"
#include "Panels\ContentBroserPanel.h"
#include "Panels/AnimationPanel.h"
#include "ImGuizmo\ImGuizmo.h"
namespace Adran
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		virtual ~EditorLayer() {}

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(TimeStep ts) override;
		void OnEvent(Event& event) override;

		void OnImGuiRender() override;

	private:
		void UIGameBar();
		void EditorGameView();
	private:
		void NewScene();
		void OpenScenePath(const std::filesystem::path& path);
		void OpenScene();
		void SaveScene();
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnMousePressedEvent(MouseButtonPressedEvent& event);
	private:
		//Tex
		Ref<Texture2D> m_AmiyaTex;
		Ref<Texture2D> m_amiyaSTex;
		Ref<Texture2D> m_suzuranTex;
		Ref<Texture2D> m_unKnownTex;
		Ref<Texture2D> m_weedyTex;

		//Renderer About
		ShaderLibrary m_shaderLibrary;
		Ref<FrameBuffer> m_frameBuffer;
		
		//Scene
		Ref<Scene> m_scene;
		Ref<Scene> m_tempScene;

		//Other
		glm::vec2 m_viewSize = glm::vec2(0.0f);
		glm::vec2 m_viewportBounds[2] = { glm::vec2(0.0f), glm::vec2(0.0f) };

		int m_ImGuizmoType = ImGuizmo::OPERATION::TRANSLATE;
		int m_horverdEntity = -1;
		Ref<SceneHierachyPanel> m_hierachyPanel;
		Ref<ContentBroserPanel> m_contentPanel;
		Ref<AnimationPanel> m_animPanel;
		Ref<EditorCamera> m_editorCamera;
	private:
		bool m_isGameViewFocused = true;
		bool m_isGameViewHorverd = false;
	private:
		//Editor Icon
		Ref<Texture2D> m_startIcon;
		Ref<Texture2D> m_pauseIcon;
	};
}
