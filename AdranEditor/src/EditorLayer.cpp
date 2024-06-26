#include "EditorLayer.h"
#include "../vendor/imgui/imgui.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "script/CameraController.h"
#include "script/SelfRotation.h"
#include <chrono>




namespace Adran {

    const std::filesystem::path g_AssetPath = "assets";

    EditorLayer::EditorLayer():Adran::Layer("Editor")
    {

    }

    void EditorLayer::OnAttach()
    {
	    /*m_particleSystem.Init(
		    500, 200,
		    0.3f, 0.1f,
		    180.0f, 0.0f,
		    0.1f, 0.06f,
		    { 0.2f, 0.2f, 0.0f }, {-0.2f, -0.2f, 0.0f},
		    -1.0f, -0.1f,
		    1.0f, -1.0f
	    );*/
	    
        //Texture Init
        {
        }
        //FrameBuffer Init
        {
            FrameBufferSpecification spec;
            spec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth};
            spec.width = 1920;
            spec.height = 1080;
            m_frameBuffer = FrameBuffer::Create(spec);
        }

        //Scene Init()
        {
            m_tempScene = nullptr;
            m_scene = CreateRef<Scene>();
            m_startIcon = Texture2D::Create("assets/Icon/start.png");
            m_pauseIcon = Texture2D::Create("assets/Icon/pause.png");
        }


       m_hierachyPanel = CreateRef<SceneHierachyPanel>(m_scene);
       m_contentPanel = CreateRef<ContentBroserPanel>();
       m_animPanel = CreateRef<AnimationPanel>();
       m_editorCamera = CreateRef<EditorCamera>((float)Application::GetInstance().GetWindow().GetWidth() / (float)Application::GetInstance().GetWindow().GetWidth());
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(TimeStep ts)
    {
        if (FrameBufferSpecification spec = m_frameBuffer->GetSpec();
            m_viewSize.x > 0.0f && m_viewSize.y > 0.0f)
        {
            m_frameBuffer->ReSize((uint32_t)m_viewSize.x, (uint32_t)m_viewSize.y);
            m_scene->SetViewport((uint32_t)m_viewSize.x, (uint32_t)m_viewSize.y);

            m_editorCamera->SetViewport(m_viewSize.x, m_viewSize.y);
        }

	    /*if (Input::IsMouseButtonPressed(AR_MOUSE_BUTTON_LEFT))
	    {
		    auto [x, y] = Input::GetMousePosition();
		    auto width = Application::Get().GetWindow().GetWidth();
		    auto height = Application::Get().GetWindow().GetHeight();

		    auto bounds = m_cameraController.GetBounds();
		    auto pos = m_cameraController.GetCamera().GetPosition();

		    x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		    y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		    m_particleSystem.SetCreatePosition({ x + pos.x, y + pos.y, 0.0f });
		    m_particleSystem.TakeMode(true);
	    }
	    else
	    {
		    m_particleSystem.TakeMode(false);
	    }*/

        //m_particleSystem.OnUpdate(ts);


	    Renderer2D::s_statics.timeStep = ts.GetMilliseconds();
	    Renderer2D::s_statics.fps = 1000.0f / ts.GetMilliseconds();

        m_frameBuffer->Bind();

        if (m_scene->GetSceneState() == Scene::SceneState::Editor)
        {
            RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
            RenderCommand::Clear();
            //ClearAttachment需要放在Scene绘制函数前面
            m_frameBuffer->ClearAttachment(1, -1);


            m_scene->OnUpdateEditor(ts, *m_editorCamera);

            //There is some bug
            if (m_isGameViewFocused)
            {
                m_editorCamera->OnUpdate(ts);
            }

            //获取鼠标位置
            auto [mx, my] = ImGui::GetMousePos();
            //光标位置减去窗口位置就可以获得光标在窗口内的位置
            mx -= m_viewportBounds[0].x;
            my -= m_viewportBounds[0].y;



            //获取viewSize
            glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
            my = viewportSize.y - my;
            int mouseX = (int)mx;
            int mouseY = (int)my;
            if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
            {
                int pixelData = m_frameBuffer->GetPixel(1, mouseX, mouseY);
                m_horverdEntity = pixelData;
                AR_CORE_INFO("Pixel: {0}", pixelData);
            }
            else
            {
                m_horverdEntity = -1;
            }
            
        }
        else
        {
            //TODO:Game
            RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            RenderCommand::Clear();
            //ClearAttachment需要放在Scene绘制函数前面
            m_frameBuffer->ClearAttachment(1, -1);
            m_scene->OnUpdate(ts);
        }

        
        m_frameBuffer->UnBind();

    }

    void EditorLayer::OnEvent(Event& event)
    {
	    EventDispatcher dispatcher(event);
        m_editorCamera->OnEvent(event);
	    dispatcher.Dispatch<KeyPressedEvent>(AR_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<MouseButtonPressedEvent>(AR_BIND_EVENT_FN(EditorLayer::OnMousePressedEvent));
    }

    bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& event)
    {
        if (event.GetRepeatCount() > 0)
            return false;

        bool ctrl = Input::IsKeyPressed(KeyCode::AR_LEFT_CONTROL);

        switch (event.GetKeyCode())
        {
        case KeyCode::AR_Q:
            if(m_isGameViewFocused)
                m_ImGuizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case KeyCode::AR_W:
            if (m_isGameViewFocused)
                m_ImGuizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        case KeyCode::AR_E:
            if (m_isGameViewFocused)
                m_ImGuizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        case KeyCode::AR_N:
            if (ctrl)
                NewScene();
            break;
        case KeyCode::AR_O:
            if (ctrl)
                OpenScene();
            break;
        case KeyCode::AR_S:
            if (ctrl)
                SaveScene();
            break;
        case KeyCode::AR_ESCAPE:
            Application::GetInstance().Close();
            break;
        default:
            break;
        }
        return true;

    }

    bool EditorLayer::OnMousePressedEvent(MouseButtonPressedEvent& event)
    {
        switch (event.GetButton())
        {
        case MouseCode::MOUSE_BUTTON_LEFT:
            if (m_horverdEntity >= 0  && !Input::IsKeyPressed(KeyCode::AR_LEFT_ALT))
                m_hierachyPanel->SetSelectedEntity(Entity{ (entt::entity)m_horverdEntity, m_scene.get() });
            else if (m_horverdEntity == -1 && m_isGameViewHorverd)
            {
                m_hierachyPanel->SetSelectedEntity(Entity());
            }
            break;
        }
        return true;
    }

    void EditorLayer::OnImGuiRender()
    {

        static bool dockSpace = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockSpace, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 200.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
                    NewScene();
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                    OpenScene();
                }
                if (ImGui::MenuItem("Save as...", "Ctrl+S")) {
                    SaveScene();
                }

                if (ImGui::MenuItem("Exit", "ESC")) {
                    Application::GetInstance().Close();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Settings");
        ImGui::Text("FPS: %d", Renderer2D::s_statics.fps);
        ImGui::Text("TimeStep:%f", Renderer2D::s_statics.timeStep);
        ImGui::Text("DrawCall:%d", Renderer2D::s_statics.drawCallTime);

        //ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_cameraEntity->GetComponent<TransformComponent>().position));

        ImGui::End();

        EditorGameView();

        UIGameBar();

        m_hierachyPanel->OnImGuiRender();
        m_contentPanel->OnImGuiRender();
        
        //ImGui官方Demo
        //ImGui::ShowDemoWindow();
        m_animPanel->OnImGuiRender();
        ImGui::End();
    }
    void EditorLayer::UIGameBar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        auto& colors = ImGui::GetStyle().Colors;

        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        ImGui::Begin("##GameBar", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
        
        float size = ImGui::GetWindowHeight() - 4.0f;
        Ref<Texture2D> icon = m_scene->GetSceneState() == Scene::SceneState::Editor ? m_startIcon : m_pauseIcon;
        
        //ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        ImGui::SetCursorPosX((size * 0.5f));
        if (ImGui::ImageButton((ImTextureID)icon->GetRenderID(), { size, size }))
        {

            if (m_scene->GetSceneState() == Scene::SceneState::Editor)
            {
                m_scene->SetSceneState(Scene::SceneState::Play);
                m_tempScene = Scene::Copy(m_scene);
            }
            else if (m_scene->GetSceneState() == Scene::SceneState::Play)
            {
                m_scene->SetSceneState(Scene::SceneState::Editor);
                m_scene = m_tempScene;
                m_hierachyPanel->SetScene(m_scene);
            }
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);

        ImGui::End();
    }
    void EditorLayer::EditorGameView()
    {
        ImGui::Begin("Scene");

        m_isGameViewFocused = ImGui::IsWindowFocused();
        m_isGameViewHorverd = ImGui::IsWindowHovered();


        auto viewportOffset = ImGui::GetCursorPos();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (m_viewSize != *((glm::vec2*)&viewportPanelSize))
        {
            m_viewSize = { viewportPanelSize.x, viewportPanelSize.y };
        }
        uint32_t texID = m_frameBuffer->GetColorAttachmentRenderID(0);
        ImGui::Image((void*)texID, ImVec2{ m_viewSize.x, m_viewSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        {
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    OpenScenePath(path);
                }
                ImGui::EndDragDropTarget();
            }
        }

        //计算这个ImGui窗口的大小和位置
        auto windowSize = ImGui::GetWindowSize();
        //minBound是窗口的位置的左上角
        ImVec2 minBound = ImGui::GetWindowPos();

        //maxBound是窗口的位置的右下角
        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };

        m_viewportBounds[0] = { minBound.x, minBound.y };
        m_viewportBounds[1] = { maxBound.x, maxBound.y };

        Entity selectedEntity = m_hierachyPanel->GetSelectedEntity();

        if (selectedEntity && m_ImGuizmoType != -1 && m_scene->GetSceneState() == Scene::SceneState::Editor)
        {
            ImGuizmo::SetOrthographic(true);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_viewportBounds[1].x - m_viewportBounds[0].x, m_viewportBounds[1].y - m_viewportBounds[0].y);

            const glm::mat4& cameraProjection = m_editorCamera->GetProjectionMatrix();
            glm::mat4 cameraView = m_editorCamera->GetViewMatrix();

            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = (glm::mat4)tc;

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_ImGuizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));

            if (ImGuizmo::IsUsing() && !Input::IsKeyPressed(KeyCode::AR_LEFT_ALT))
            {
                glm::vec3 position = { tc.position.x, tc.position.y, 0.0f}
                    , rotation = {0.0f, 0.0f, tc.rotation}
                , scale = { tc.scale.x, tc.scale.y, 1.0f };

                Math::DecomTransform(transform, position, rotation, scale);

                tc.position = { position.x, position.y};
                tc.rotation = rotation.z;
                tc.scale = {scale.x, scale.y};
            }
        }

        ImGui::End();
    }
    void EditorLayer::NewScene()
    {
        m_scene = CreateRef<Scene>();
        m_scene->SetViewport(Application::GetInstance().GetWindow().GetWidth(), Application::GetInstance().GetWindow().GetHeight());
        m_hierachyPanel->SetScene(m_scene);
    }

    void EditorLayer::OpenScenePath(const std::filesystem::path& path)
    {
        m_scene = CreateRef<Scene>();
        m_scene->SetViewport(Application::GetInstance().GetWindow().GetWidth(), Application::GetInstance().GetWindow().GetHeight());
        m_hierachyPanel->SetScene(m_scene);

        SceneSerialize serializer(m_scene);
        serializer.DeSerialize(path.string());
    }

    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Adran Scene (*.adran)\0*.adran\0");
        if (!filepath.empty())
        {

            OpenScenePath(filepath);
        }
    }
    void EditorLayer::SaveScene()
    {
        std::string filepath = FileDialogs::SaveFile("Adran Scene (*.adran)\0*.adran\0");
        if (!filepath.empty())
        {
            SceneSerialize serializer(m_scene);
            serializer.Serialize(filepath);
        }
    }
}
