#include "AdranPreCompile.h"
#include "Model.h"
#include "Adran/Core/AssetsManager.h"
namespace Adran
{
	inline static glm::mat4 assimpToGlmMatrix(aiMatrix4x4 mat) {
		glm::mat4 m;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m[x][y] = mat[y][x];
			}
		}
		return m;
	}

	void Model::BindBone(const Ref<Shader>& shader)
	{
		shader->Bind();
		glm::mat4 boneArray[200];
		//boneArray[0] = glm::mat4(1.0f);
		for (auto& bone : m_boneMap)
		{
			boneArray[bone.second->id] = bone.second->offset;
		}
		shader->UploadUniformMat4Array("u_boneTransform", boneArray, m_boneMap.size());
	}
	
	Model::Model(const Ref<Mesh>&mesh)
	{
		m_Meshes.push_back(*mesh);
	}
	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			AR_CORE_ERROR("Assimp报错:{0}", importer.GetErrorString());
			return;
		}
		m_path = path;
		m_directory = path.substr(0, path.find_last_of('\\'));

		ProcessNode(scene->mRootNode, scene);

		// create bone hirerchy
		Ref<Bone> nul;
		readSkeleton(nul, scene->mRootNode);

		for (const auto& bone : m_boneMap)
		{
			for (const auto& children : bone.second->chilren)
			{
				if (children != nullptr)
				{
					AR_ERROR("The Bonde: {0} Have children :{1}", bone.second->name, children->name);
				}
			}
		}
	}
	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		Ref<Material> material;

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			aiVector3D& aiVertex = mesh->mVertices[i];
			vertex.Position = { aiVertex.x, aiVertex.y, aiVertex.z };
			aiVector3D& aiNormal = mesh->mNormals[i];
			vertex.Normal = { aiNormal.x, aiNormal.y, aiNormal.z };
			if (mesh->mTextureCoords[0])  // has tex coord?
			{
				aiVector3D& aiTexCoord = mesh->mTextureCoords[0][i];
				vertex.TexCoord = { aiTexCoord.x, aiTexCoord.y };
			}
			else
			{
				vertex.TexCoord = { 0.0f, 0.0f };
			}
			vertices.push_back(vertex);
		}

		//此处开始处理mesh的骨骼
		{
			//开始处理bone
			std::vector<uint32_t> boneCounts;
			boneCounts.resize(mesh->mNumVertices, 0);
			uint32_t nBoneCount = mesh->mNumBones;
			//开始获取bone数据
			for (uint32_t i = 0; i < nBoneCount; i++) {
				aiBone* bone = mesh->mBones[i];
				//glm::mat4 m = assimpToGlmMatrix(bone->mOffsetMatrix);
				glm::mat4 m = glm::mat4(0.0f);
				
				Bone theBone = Bone{ i, bone->mName.C_Str(),m };
				if (m_boneMap.find(theBone.name) != m_boneMap.end())
				{
					
				}
				else
				{
					m_boneMap[bone->mName.C_Str()] = CreateRef<Bone>(theBone);
				}

				//开始遍历这跟骨头涉及到的顶点的权重
				for (int j = 0; j < bone->mNumWeights; j++) {
					uint32_t id = bone->mWeights[j].mVertexId;
					float weight = bone->mWeights[j].mWeight;
					boneCounts[id]++;
					switch (boneCounts[id]) {
					case 1:
						vertices[id].boneIds.x = i;
						vertices[id].boneWeights.x = weight;
						break;
					case 2:
						vertices[id].boneIds.y = i;
						vertices[id].boneWeights.y = weight;
						break;
					case 3:
						vertices[id].boneIds.z = i;
						vertices[id].boneWeights.z = weight;
						break;
					case 4:
						vertices[id].boneIds.w = i ;
						vertices[id].boneWeights.w = weight;
						break;
					default:
						//std::cout << "err: unable to allocate bone to vertex" << std::endl;
						break;

					}
				}
			}

			//标准化，让一个顶点对其关联的所有骨骼的权重总合为1
			for (int i = 0; i < vertices.size(); i++) {
				glm::vec4& boneWeights = vertices[i].boneWeights;
				float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
				if (totalWeight > 0.0f) {
					vertices[i].boneWeights = glm::vec4(
						boneWeights.x / totalWeight,
						boneWeights.y / totalWeight,
						boneWeights.z / totalWeight,
						boneWeights.w / totalWeight
					);
				}
			}

		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
			//AR_CORE_ERROR((int)mat->GetTextureCount(aiType));
			material = LoadMaterial(aiMaterial);
		}

		return Mesh(vertices, indices, material);
	}
	Ref<Material> Model::LoadMaterial(aiMaterial* mat)
	{

		Ref<Material> material = CreateRef<Material>();
		material->SetName(mat->GetName().C_Str());
		constexpr uint32_t numTexTypes = 3;
		std::array<aiTextureType, numTexTypes> aiTypes = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_EMISSIVE};
		std::array<Texture::Type, numTexTypes> types = { Texture::Type::Diffuse, Texture::Type::Specular, Texture::Type::Emission};


		//ERROR HERE
		for (uint32_t TexTypeIdx = 0; TexTypeIdx < numTexTypes; TexTypeIdx++)
		{
			aiTextureType aiType = aiTypes[TexTypeIdx];
			Texture::Type type = types[TexTypeIdx];

			for (uint32_t i = 0; i < (uint32_t)mat->GetTextureCount(aiType); i++)
			{
				//AR_ERROR(TexTypeIdx);
				aiString str;
				mat->GetTexture(aiType, i, &str);

				std::string path = m_directory + '/' + str.C_Str();
				Ref<Texture2D> texture = std::dynamic_pointer_cast<Texture2D>(AssetsManager::GetInstance()->GetTexture2D(path));
				if (texture == nullptr)
				{
					texture = Texture2D::Create("assets/textures/casual.png");
					material->SetTextureMap(texture, type);
				}
				else
				{
					material->SetTextureMap(texture, type);
				}
			}
		}
		return material;
	}

	Ref<Model> Model::CreateDefaultModel(const Ref<Texture2D>& texture)
	{
		Ref<Material> DefaultMaterial = CreateRef<Material>();
		DefaultMaterial->SetTextureMap(texture, Texture::Type::Diffuse);

		std::vector<Vertex> CubeVertex = {
			Vertex{ {-0.5f, -0.5f, 0.5f}, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			Vertex{ { 0.5f, -0.5f, 0.5f}, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			Vertex{ { 0.5f,  0.5f, 0.5f}, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
			Vertex{ {-0.5f,  0.5f, 0.5f}, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
			Vertex{ {-0.5f, -0.5f,-0.5f}, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			Vertex{ { 0.5f, -0.5f,-0.5f}, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			Vertex{ { 0.5f,  0.5f,-0.5f}, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
			Vertex{ {-0.5f,  0.5f,-0.5f}, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } }
		};

		std::vector<uint32_t> indecise = {
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			3, 6, 7,
			3, 6, 2,
			0, 4, 5,
			0, 1, 5,
			1, 5, 2,
			2, 6, 5,
			0, 4, 3,
			4, 3, 7
		};

		auto cubeMesh = CreateRef<Mesh>(CubeVertex, indecise, DefaultMaterial);
		return CreateRef<Model>(cubeMesh);
	}

	//Bone About

	bool Model::readSkeleton(Ref<Bone>& outChilren, aiNode* node)
	{
		//判断当前结点是否有骨头
		if (m_boneMap.find(node->mName.C_Str()) != m_boneMap.end()) { 
			outChilren = m_boneMap[node->mName.C_Str()];
			for (int i = 0; i < node->mNumChildren; i++) {
				Ref<Bone> chilren = nullptr;
				readSkeleton(chilren, node->mChildren[i]);
				//查找当前骨骼的字节点的chilren
				if (chilren != nullptr)
				{
					m_boneMap[node->mName.C_Str()]->chilren.push_back(chilren);
				}
					
			}
			return true;
		}
		else { // find bones in children
			
			for (int i = 0; i < node->mNumChildren; i++) {
				if (readSkeleton(outChilren, node->mChildren[i])) {
					return true;
				}

			}
		}
		return false;
	}


}