#pragma once

#include "Adran/Renderer/RenderObject/Shader.h"
#include <string>
#include <glad/glad.h>

namespace Adran {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filePath);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const std::string& GetName() const override { return m_name; }

		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix)override;
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;
		virtual void UploadUniformMat4Array(const std::string& name, glm::mat4 matrix[], uint32_t count)override;
		virtual void UploadUniformFloat(const std::string& name, float value) override;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& values) override;
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) override;
		virtual void UploadUniformInt(const std::string& name, int value) override;
		virtual void UploadUniformIntArray(const std::string& name, int* values, uint32_t count) override;
	private:
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSource);
	private:
		uint32_t m_renderID;
		std::string m_name;
	};
}