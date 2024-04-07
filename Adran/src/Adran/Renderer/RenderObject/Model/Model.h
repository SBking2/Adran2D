#pragma once

#include "../../../Core/Core.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Material.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Adran
{
	//Model在创建时会默认设置model

	class Model
	{
	public:
		struct Bone {
			uint32_t id = 0;
			std::string name = "";
			glm::mat4 offset = glm::mat4(1.0f);
			glm::vec3 position = glm::vec3(0.0f);
			glm::vec3 rotation = glm::vec3(0.0f);
			glm::vec3 scale = glm::vec3(1.0f);

			std::vector<Ref<Bone>> chilren;

			void ReCulate()
			{
				for (auto& chil : chilren)
				{
					if (chil != nullptr)
					{
						chil->position = position;
						chil->rotation = rotation;
						chil->scale = scale;
						chil->offset = offset;
						chil->ReCulate();
					}
				}
			}

		};
	public:
		Model() = delete;
		Model(const Ref<Mesh>& mesh);
		Model(const std::string& path)
		{
			LoadModel(path);
			AR_ERROR(m_boneMap.size());
			for (auto& bone : m_boneMap)
			{
				AR_ERROR(bone.second->name);
				AR_INFO(bone.second->id);
			}

		}
		const std::string& GetPath()const
		{
			return m_path;
		}
		const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }

		std::unordered_map<std::string, Ref<Bone>>& GetBoneMap()
		{
			return m_boneMap;
		}

		void BindBone(const Ref<Shader>& shader);

		static Ref<Model> CreateDefaultModel(const Ref<Texture2D>& texture);
	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		Ref<Material> LoadMaterial(aiMaterial* mat);
	private:
		bool readSkeleton(Ref<Bone>& outChilren, aiNode* node);
	private:
		std::vector<Mesh> m_Meshes;
		std::string m_directory;
		std::string m_path;
	private:
		std::unordered_map<std::string, Ref<Bone>> m_boneMap;
	};
}