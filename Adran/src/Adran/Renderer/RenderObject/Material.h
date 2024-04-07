#pragma once

#include "Adran\Core\Core.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
namespace Adran
{

	
	class Material
	{
	public:
		Material(
			Ref<Texture2D> diffuseMap = nullptr,
			Ref<Texture2D> specularMap = nullptr,
			Ref<Texture2D> emissionMap = nullptr,
			const std::string& path = "NULL"
		);

		void SetTextureMap(Ref<Texture2D> texture, Texture::Type type);
		Ref<Texture2D> GetTextureMap(Texture::Type type);

		inline float GetShininess() const { return m_shininess; }
		inline void SetShininess(float shininess) { m_shininess = shininess; }
		inline const std::string& GetPath() { return m_path; }
		inline const std::string& GetName() { return m_name; }
		inline void SetName(const std::string& name) { m_name = name; }

		inline bool GetDiffuseUse() { return m_isDiffuseUse; }
		inline void SetDiffuseUse(bool cond) { m_isDiffuseUse = cond; }
		inline bool GetSpecularUse() { return m_isSpecularUse; }
		inline void SetSpecularUse(bool cond) { m_isSpecularUse = cond; }
		inline glm::vec4& GetDiffuseColor() { return m_diffuseColor; }


		void Bind(const Ref<Shader>& shader);
	private:
		static const int s_diffuseSlot = 0;
		static const int s_sepcularSlot = 1;
		static const int s_emissionSlot = 2;
	private:
		Ref<Texture2D> m_defaultTex;
		Ref<Texture2D> m_diffuseMap;
		Ref<Texture2D> m_specularMap;
		Ref<Texture2D> m_emissionMap;
		
		glm::vec4 m_diffuseColor;
		bool m_isDiffuseUse = true;
		bool m_isSpecularUse = true;

		std::string m_name;
		std::string m_path;
		float m_shininess;
	};

}