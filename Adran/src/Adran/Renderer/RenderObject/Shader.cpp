#include "AdranPreCompile.h"
#include "Shader.h"

#include "Adran/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Adran {
	
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::NONE:		AR_CORE_ASSERT(false, "渲染API为NONE！"); break;
		case RenderAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc); break;
		}
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::NONE:		AR_CORE_ASSERT(false, "渲染API为NONE！"); break;
		case RenderAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(filePath); break;
		}
		AR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}


	// ------------------------------------- ShaderLibrary Define -----------------------------
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		AR_CORE_ASSERT(!Exists(name), "着色器已经存在!");
		m_shaders[name] = shader;
	}

	Ref<Adran::Shader> ShaderLibrary::Get(const std::string& name)
	{
		AR_CORE_ASSERT(Exists(name), "着色器不存在!");
		return m_shaders[name];
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		auto shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(shader);
		return shader;
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_shaders.find(name) != m_shaders.end();
	}

}

