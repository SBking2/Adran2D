#pragma once
#include "Adran/Renderer/Camera/SceneCamera.h"
#include "Script.h"
#include "Adran/Renderer/RenderObject/Texture.h"
#include "Adran/Core/UUID.h"
#include "Adran/Animation/Animation.h"

#include<glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Adran
{
	struct IDComponent 
	{
		UUID UID;
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct RenderNumComponent
	{
		uint32_t number = 0;
		RenderNumComponent() = default;
		RenderNumComponent(const RenderNumComponent&) = default;
	};

	struct TransformComponent
	{
		glm::vec2 position = { 0.0f, 0.0f};
		float rotation = 0.0f;
		glm::vec2 scale = { 1.0f, 1.0f};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec2& position, float rotation, const glm::vec2& scale) :
			position(position), rotation(rotation), scale(scale) {  }
		operator const glm::mat4& () {

			return glm::translate(glm::mat4(1.0f), glm::vec3{ position.x, position.y, 0.0f})
				* glm::rotate(glm::mat4(1.0f), rotation, {0.0f , 0.0f, 1.0f})
				* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));
		}
	};

	struct NameComponent
	{
		std::string name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name = "GameObject") :name(name) {  }
		operator std::string& () { return name; }
		operator const std::string& () { return name; }
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool isPrimary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(float aspectRatio) :camera(aspectRatio) {  }
		operator Adran::Camera& () { return camera; }
		operator const Adran::Camera& () { return camera; }
	};

	struct SpriteComponent 
	{
		Ref<Texture2D> texture = nullptr;
		glm::vec2 minTexCoords = { 0.0f, 0.0f };
		glm::vec2 maxTexCoords = { 1.0f, 1.0f };
		glm::vec4 color = glm::vec4(1.0f);

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const Ref<Texture2D>& texture) :texture(texture) {  }
		operator Ref<Texture2D>& () { return texture; }
	};

	struct ScriptComponent
	{
		Script* instance = nullptr;

		Script*(*InitalScript)();
		void (*Destroy)(ScriptComponent*);

		template<typename T>
		void Bind()
		{
			InitalScript = []() {
				Script* result = new T();
				return result;
			};

			Destroy = [](ScriptComponent* sc) {
				delete sc->instance;
				sc->instance = nullptr;
			};
		}
	};

	struct AnimationComponent
	{
		Ref<Animation> animation;
		AnimationComponent() = default;
		AnimationComponent(const AnimationComponent&) = default;
		AnimationComponent(float duration, uint32_t samples)
		{
			animation = CreateRef<Animation>(duration, samples);
		};
		AnimationComponent(const Ref<Animation>& animation) :animation(animation) {  }
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
		ComponentGroup<TransformComponent, SpriteComponent
		, CameraComponent, ScriptComponent,
		SpriteComponent, AnimationComponent>;
}
