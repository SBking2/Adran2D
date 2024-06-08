#include "pingtuLayer.h"
#include "../vendor/imgui/imgui.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <chrono>

namespace Adran {

    pingtuLayer::pingtuLayer():Adran::Layer("pingtu")
    {

    }

    void pingtuLayer::OnAttach()
    {
        //Scene Init
        {

            m_scene = CreateRef<Scene>();

            auto& cameraEntity = m_scene->CreateEntity("Camera");
            cameraEntity.AddComponent<CameraComponent>();
        }

        //orgin Texture Init
        
        m_orginTex = AssetsManager::GetInstance()->GetTexture2D("assets/textures/sport.jpg");

        uint32_t width = m_orginTex->GetWidth();
        uint32_t height = m_orginTex->GetHeight();

        auto& orEntity = m_scene->CreateEntity("orginTex");
        orEntity.AddComponent<SpriteComponent>().texture = m_orginTex;

        auto& amiya = m_scene->CreateEntity("Amiya");
        amiya.AddComponent<SpriteComponent>();
        auto& anim = AnimationSerialize::DeSerializeAnimation("assets/Animation/Animation.aa");
        amiya.AddComponent<AnimationComponent>(anim);
        amiya.GetComponent<TransformComponent>().position = {3.0f, -1.5f};

        float Xscale = 0.5f;
        float Yscale = 0.5f;

        float aspectRatio = (float)width / (float)height;

        float primary_width = 4.0f;
        float primary_height = 4.0f;

        if (width > height)
        {
            Xscale = Yscale * aspectRatio;
            primary_height = primary_width / aspectRatio;
        }
        else
        {
            Yscale = Xscale / aspectRatio;
            primary_width = primary_height * aspectRatio;
        }

        orEntity.GetComponent<TransformComponent>().scale = { Xscale, Yscale };
        orEntity.GetComponent<TransformComponent>().position = { -3.0f, 2.0f };
        
        m_xOffset = primary_width / m_verticalPi;
        m_yOffset = primary_height / m_horizalPi;

        for (int i = 0; i < m_verticalPi; i++)
        {
            for (int j = 0; j < m_horizalPi; j++)
            {
                float widthRange = 2.5f;
                float heightRange = 1.8f;
                float x = Random::Float() * widthRange * 2 - widthRange;
                float y = Random::Float() * heightRange * 2 - heightRange;

                auto& entity = m_scene->CreateEntity();

                float x_ratio = 1.0f / (float)m_verticalPi;
                float y_ratio = 1.0f / (float)m_horizalPi;

                auto subTex = CreateRef<SubTexture>(m_orginTex, glm::vec2((i + 1) * x_ratio, 1 - j * y_ratio), glm::vec2(i * x_ratio, 1 - (j + 1) * y_ratio));

                entity.AddComponent<SpriteComponent>().texture = subTex->GetTexture();
                entity.GetComponent<SpriteComponent>().minTexCoords = subTex->GetCoords()[0];
                entity.GetComponent<SpriteComponent>().maxTexCoords = subTex->GetCoords()[2];

                entity.GetComponent<TransformComponent>().scale = { m_xOffset, m_yOffset };
                entity.GetComponent<TransformComponent>().position = { x, y };

                m_subEntities.push_back({ i, j, entity.GetComponent<TransformComponent>().position, entity });
            }
        }

        //Slot Init
        for (int i = 0; i < m_verticalPi; i++)
        {
            for (int j = 0; j < m_horizalPi; j++)
            {
                auto& entity = m_scene->CreateEntity();
                entity.AddComponent<SpriteComponent>().color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
                entity.GetComponent<TransformComponent>().position = glm::vec2{ m_xOffset * i, - m_yOffset * j }
                - glm::vec2{ primary_width / 2, -primary_height / 2 }
                + glm::vec2{ m_xOffset /2, -m_yOffset /2 };
                entity.GetComponent<TransformComponent>().scale = { m_xOffset - 0.05f, m_yOffset - 0.05f };
                m_slotEntities.push_back({ i, j, entity.GetComponent<TransformComponent>().position, entity });
            }
        }
        
        m_scene->SetSceneState(Scene::SceneState::Play);
    }

    void pingtuLayer::OnDetach()
    {

    }

    void pingtuLayer::OnUpdate(TimeStep ts)
    {
        uint32_t win_width = Application::GetInstance().GetWindow().GetWidth();
        uint32_t win_height = Application::GetInstance().GetWindow().GetHeight();

        float aspectRatio = (float)win_width / (float)win_height;

        auto [x, y] = Input::GetMousePosition();

        x = (x / win_width) * (2 * aspectRatio * 2.5f) - (2 * aspectRatio * 2.5f) * 0.5f;
        y = (2 * 2.5f) * 0.5f - (y / win_height) * (2 * 2.5f);
        //互动代码
        if (Input::IsMouseButtonPressed(AR_MOUSE_BUTTON_LEFT))
        {

            if (!m_handleSlot)
            {
                m_mouse_delta = { x, y };
                float minDistance = 100.0f;
                //获取最近slot
                for (auto& slot : m_subEntities)
                {
                    //AR_ERROR("Mouse Position:{0} {1}", slot.position.x, slot.position.y);
                    if (std::abs(x - slot.position.x) <= m_xOffset / 2 && std::abs(y - slot.position.y) <= m_yOffset/2 && slot.isLive)
                    {
                        float dx = x - slot.position.x;
                        float dy = y - slot.position.y;
                        float distance = sqrt(dx * dx + dy * dy);

                        if (distance < minDistance)
                        {
                            minDistance = distance;
                            m_handleSlot = &slot;
                        }
                    }

                }
            }
            else
            {
                glm::vec2 delta = glm::vec2{ x, y } - m_mouse_delta;
                m_mouse_delta = { x,y };
                m_handleSlot->entity.GetComponent<TransformComponent>().position += delta;
                m_handleSlot->position = m_handleSlot->entity.GetComponent<TransformComponent>().position;
            }

        }
        else
        {
            m_mouse_delta = { x, y };
            if (m_handleSlot)
            {
                for (auto slot : m_slotEntities)
                {
                    if (m_handleSlot->i == slot.i && m_handleSlot->j == slot.j)
                    {
                        float dx = m_handleSlot->position.x - slot.position.x;
                        float dy = m_handleSlot->position.y - slot.position.y;
                        float distance = sqrt(dx * dx + dy * dy);
                        if (distance < 0.1f)
                        {
                            //从m_subEntitise删除
                            m_handleSlot->entity.GetComponent<TransformComponent>().position = slot.position;
                            m_handleSlot->isLive = false;
                            m_handleSlot->position = slot.position;
                            AR_ERROR("Success");
                        }
                    }
                }
            }
            m_handleSlot = nullptr;
        }

        m_scene->SetViewport(win_width, win_height);
        //TODO:Game
        RenderCommand::SetClearColor({ 0.5f, 0.5f, 0.5f, 1.0f });
        RenderCommand::Clear();

        m_scene->OnUpdate(ts);
    }

    void pingtuLayer::OnEvent(Event& event)
    {
	    EventDispatcher dispatcher(event);
	    //dispatcher.Dispatch<KeyPressedEvent>(AR_BIND_EVENT_FN(pingtuLayer::OnKeyPressedEvent));
        //dispatcher.Dispatch<MouseButtonPressedEvent>(AR_BIND_EVENT_FN(pingtuLayer::OnMousePressedEvent));
    }


    void pingtuLayer::OnImGuiRender()
    {

    }
}
