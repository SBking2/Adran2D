#include "AdranPreCompile.h"
#include "Renderer3D.h"
#include "../RenderObject/Light.h"
#include "../RenderCommand.h"

#include<glm/gtc/matrix_transform.hpp>
#include "Adran\Core\Core.h"
#include <glm/glm.hpp>
#include "../RenderObject/Model/Model.h"

namespace Adran
{
	struct Renderer3DStorage
	{
		Ref<Texture2D> greenTexture;
		Ref<Shader> TextureShader;
	};

	static Renderer3DStorage s_Data;

	void Renderer3D::Init()
	{
		s_Data.TextureShader = Shader::Create("../AdranEditor/assets/Shader/3DShader.glsl");
		s_Data.TextureShader->Bind();
	}

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	void Renderer3D::BeginScene(const EditorCamera& camera, const Ref<Light>& light, const glm::vec3& viewPos)
	{
		glm::mat4 viewProj = camera.GetViewProjection();
		s_Data.TextureShader->Bind();
		light->Bind(s_Data.TextureShader);
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection", viewProj);
		s_Data.TextureShader->UploadUniformFloat3("u_viewPosition", viewPos);
		//StartBatch();
	}

	void Renderer3D::EndScene()
	{
		Flush();
	}

	void Renderer3D::Flush()
	{
	}

	void Renderer3D::DrawVertexArray(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());
	}

	void Renderer3D::DrawModelEntity(const glm::mat4& transform, int EntityID, const Ref<Model>& model)
	{

		s_Data.TextureShader->UploadUniformMat4("u_Transform", transform);
		s_Data.TextureShader->UploadUniformInt("u_EntityID", EntityID);


		if (model)
		{
			model->BindBone(s_Data.TextureShader);
			for (auto& mesh : model->GetMeshes())
			{
				if (mesh.GetMaterial())
				{
					mesh.GetMaterial()->Bind(s_Data.TextureShader);
				}
				DrawVertexArray(mesh.GetVertexArray());
			}
		}
	}
}