#pragma once
#include "Adran/Renderer/Camera/SceneCamera.h"
#include "Script.h"
#include "Adran/Renderer/RenderObject/Texture.h"
#include "Adran/Renderer/RenderObject/Model/Model.h"
#include "Adran/Renderer/RenderObject/Model/Animation.h"
#include "Adran/Core/UUID.h"

#include<glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Adran
{
	struct IDComponent 
	{
		UUID UID;
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TransformComponent
	{
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::quat rotation = { 0.0f, 0.0f, 0.0f, 0.0f};
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
			position(position), rotation(rotation), scale(scale) {  }
		operator const glm::mat4& () {

			return glm::translate(glm::mat4(1.0f), position)
				* glm::toMat4(rotation)
				* glm::scale(glm::mat4(1.0f), scale);
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

	struct ModelComponent
	{
		Ref<Model> model = nullptr;

		ModelComponent() = default;
		ModelComponent(const ModelComponent&) = default;
		ModelComponent(const Ref<Model>& model) :model(model) {  }
		operator Ref<Model>& () { return model; }
	};

	struct AnimationComponent
	{
		Ref<Animation> animation = nullptr;

		AnimationComponent() = default;
		AnimationComponent(const AnimationComponent&) = default;
		AnimationComponent(const Ref<Animation>& animation) :animation(animation) {  }
		operator Ref<Animation>& () { return animation; }
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


}
