#include "AdranPreCompile.h"
#include "Material.h"


namespace Adran
{
	Material::Material(Ref<Texture2D> diffuseMap
		, Ref<Texture2D> specularMap
		, Ref<Texture2D> emissionMap
		, const std::string& path)
	{
		m_diffuseColor = glm::vec4(1.0f);
		uint32_t color = 0xffffffff;
		m_defaultTex = Texture2D::CreateFlatColor(1, 1, &color);

		m_diffuseMap = !diffuseMap ? m_defaultTex : diffuseMap;
		m_specularMap = !specularMap ? m_defaultTex : specularMap;
		m_emissionMap = !emissionMap ? m_defaultTex : emissionMap;
		m_path = path;
	}


	void Material::SetTextureMap(Ref<Texture2D> texture, Texture::Type type)
	{
		switch (type)
		{
		case Adran::Texture::Diffuse:
			m_diffuseMap = !texture ? m_defaultTex : texture;
			break;
		case Adran::Texture::Specular:
			m_specularMap = !texture ? m_defaultTex : texture;
			break;
		case Adran::Texture::Emission:
			m_emissionMap = !texture ? m_defaultTex : texture;
			break;
		default:
			break;
		}
	}
	Ref<Texture2D> Material::GetTextureMap(Texture::Type type)
	{
		switch (type)
		{
		case Adran::Texture::Diffuse:
			return m_diffuseMap;
			break;
		case Adran::Texture::Specular:
			return m_specularMap;
			break;
		case Adran::Texture::Emission:
			return m_emissionMap;
			break;
		default:
			break;
		}
	}

	void Material::Bind(const Ref<Shader>& shader)
	{
		shader->Bind();
		// diffuse
		AR_CORE_ASSERT(m_diffuseMap, "材质没有纹理贴图!");

		m_isDiffuseUse ? m_diffuseMap->Bind(s_diffuseSlot) : m_defaultTex->Bind(s_diffuseSlot);

		shader->UploadUniformInt("u_diffuse", s_diffuseSlot);
		shader->UploadUniformFloat4("u_diffuseColor", m_diffuseColor);
		// specular

		m_isSpecularUse ? m_specularMap->Bind(s_sepcularSlot) : m_defaultTex->Bind(s_sepcularSlot);

		shader->UploadUniformInt("u_specular", s_sepcularSlot);
		shader->UploadUniformFloat("u_shininess", m_shininess);
		// emission
		m_emissionMap->Bind(s_emissionSlot);
		shader->UploadUniformInt("u_emission", s_emissionSlot);

		m_diffuseMap->Bind(s_diffuseSlot);
	}
}