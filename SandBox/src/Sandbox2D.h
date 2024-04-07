#pragma once
#include <Adran.h>
#include "ParticleSystem.h"
class Sandbox2DLayer : public Adran::Layer
{
public:
	Sandbox2DLayer();

	virtual ~Sandbox2DLayer() {}

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Adran::TimeStep ts) override;
	void OnEvent(Adran::Event& event) override;
	bool OnKeyPressedEvent(Adran::KeyPressedEvent& event);
	void OnImGuiRender() override;
private:

	Adran::Ref<Adran::Texture2D> m_AmiyaTex;
	Adran::Ref<Adran::Texture2D> m_RPGBaseTex;
	Adran::Ref<Adran::SubTexture> m_solidSubTex;
	Adran::ShaderLibrary m_shaderLibrary;
	Adran::OrthographicCameraController m_cameraController;
	ParticleSystem m_particleSystem;
};