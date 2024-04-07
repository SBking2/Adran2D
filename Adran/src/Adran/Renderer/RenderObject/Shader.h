#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Adran {
	//每个Shader都有名字,若使用文件创建Shader,则由文件名作为Shader名
	//若由vertexSrc & fragmentSrc 创建,则Shader名自己指定。
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& filePath);

		virtual void UploadUniformMat4(const std::string & name, const glm::mat4& matrix) = 0;
		virtual void UploadUniformMat4Array(const std::string & name, glm::mat4 matrix[], uint32_t count) = 0;
		virtual void UploadUniformMat3(const std::string & name, const glm::mat3& matrix) = 0;
		virtual void UploadUniformFloat(const std::string& name, float value) = 0;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& values) = 0;
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) = 0;
		virtual void UploadUniformInt(const std::string& name, int value) = 0;
		virtual void UploadUniformIntArray(const std::string& name, int* values, uint32_t count) = 0;
	};
	class ShaderLibrary
	{
	public:
		//添加已有Shader
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Get(const std::string& name);
		//创建新Shader();
		Ref<Shader> Load(const std::string& filePath);
		Ref<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		//判断是否存在Shader
		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};
}