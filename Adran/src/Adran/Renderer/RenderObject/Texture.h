#pragma once

#include <string>
#include "Adran/Core/Core.h"
#include<glm/glm.hpp>
namespace Adran
{
	class Texture
	{
	public:
		enum Type {
			Diffuse,
			Specular,
			Emission
		};

		virtual ~Texture(){}

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRenderID() const = 0;
		virtual const std::string& GetPath()const = 0;
		virtual void SetPath(const std::string& path) = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> CreateFlatColor(uint32_t width, uint32_t height, void* data);
	};
}