#include "AdranPreCompile.h"
#include "light.h"

namespace Adran
{
	Light::Light(const glm::vec3& position, float strength)
		:m_position(position), m_strength(strength)
	{

	}
	void Light::Bind(const Ref<Shader>& shader)
	{
		shader->Bind();

		shader->UploadUniformFloat3("u_LightDire", m_position);
		shader->UploadUniformFloat("u_LightStrength", m_strength);
	}
}