#include "AdranPreCompile.h"
#include "Renderer2D.h"

#include "../RenderObject/VertexArray.h"
#include "../RenderObject/Shader.h"
#include "../RenderCommand.h"

#include<glm/gtc/matrix_transform.hpp>

namespace Adran
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexSlot;
		float TexTiling;
		int EntityID = -1;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuadNum = 10000;
		const uint32_t MaxVertices = MaxQuadNum * 4;
		const uint32_t MaxIndices = MaxQuadNum * 6;
		uint32_t IndicesCount = 0;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> whiteTexture;

		QuadVertex* QuadVertexPtr = nullptr;
		QuadVertex* QuadVertexBase = nullptr;

		//纹理
		static const uint32_t MaxTexNum = 32;
		std::array<Ref<Texture2D>, MaxTexNum> TexSlots;
		uint32_t TexIndex = 1;//0是白色纹理

		glm::vec4 QuadVertexPosition[4] =
		{
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f,  0.5f, 0.0f, 1.0f},
			{-0.5f,  0.5f, 0.0f, 1.0f}
		};
	};

	static Renderer2DData s_Data;

	Statics Renderer2D::s_statics(s_Data.MaxQuadNum);

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();
		
		//Create Vertex Buffer
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_Color", ShaderDataType::Float4 },
			{ "a_TexCoord", ShaderDataType::Float2 },
			{ "a_TexSlot", ShaderDataType::Float},
			{ "a_TexTiling", ShaderDataType::Float},
			{ "a_EntityID", ShaderDataType::Int}
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBase = new QuadVertex[s_Data.MaxVertices];

		//Create Index Buffer
		uint32_t* QuadIndices = new uint32_t[s_Data.MaxIndices];
		//设置索引
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			QuadIndices[i + 0] = offset + 0;
			QuadIndices[i + 1] = offset + 1;
			QuadIndices[i + 2] = offset + 2;

			QuadIndices[i + 3] = offset + 2;
			QuadIndices[i + 4] = offset + 3;
			QuadIndices[i + 5] = offset + 0;
			offset += 4;
		}
		Ref<IndexBuffer> QuadIB = IndexBuffer::Create(QuadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(QuadIB);
		delete[] QuadIndices;

		uint32_t whiteTextureData = 0xffffffff;
		s_Data.whiteTexture = Texture2D::CreateFlatColor(1, 1, &whiteTextureData);

		//纹理处理初始化
		for (uint32_t i = 0; i < s_Data.TexSlots.size(); i++)
		{
			s_Data.TexSlots[i] = s_Data.whiteTexture;
		}

		s_Data.TextureShader = Shader::Create("assets/Shader/FlatColorShader.glsl");
		s_Data.TextureShader->Bind();

		int32_t samplers[s_Data.MaxTexNum];
		for (int32_t i = 0; i < s_Data.MaxTexNum; i++)
		{
			samplers[i] = i;
		}
		s_Data.TextureShader->UploadUniformIntArray("u_Textures", samplers, s_Data.MaxTexNum);
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();

		s_statics.drawCallTime = 0;
	}
	void Renderer2D::BeginScene(const SceneCamera& camera, const glm::mat4& transform)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection", camera.GetProjection() * glm::inverse(transform));

		StartBatch();

		s_statics.drawCallTime = 0;
	}

	void Renderer2D::BeginScene(const glm::mat4& transform)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection", glm::mat4(0.0f));

		StartBatch();

		s_statics.drawCallTime = 0;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		glm::mat4 viewProj = camera.GetViewProjectionMatrix();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection",  viewProj);

		StartBatch();
	}

	//绑定数据兵绘制
	void Renderer2D::EndScene()
	{
		//此处是把ptr指针减去Base计算ptr移动的长度
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexPtr - (uint8_t*)s_Data.QuadVertexBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBase, dataSize);

		for (uint32_t i = 0; i < s_Data.TexIndex; i++)
		{
			s_Data.TexSlots[i]->Bind(i);
		}

		Flush();
	}

	
	void Renderer2D::DrawQuadEntity(const glm::mat4& transform, const glm::vec4& color, int EntityID, const Ref<Texture2D>& texture)
	{
		DrawQuadEntity(transform, color, EntityID, { 0.0f, 0.0f }, {1.0f, 1.0f}, texture);
		
	}
	void Renderer2D::DrawQuadEntity(const glm::mat4& transform, const glm::vec4& color, int EntityID, const glm::vec2& minTexCoord, const glm::vec2& maxTexCoord
		, const Ref<Texture2D>& texture)
	{
		float texIndex = 0.0f;
		if (texture == nullptr)
		{

		}
		else
		{
			//图片插槽
			{

				for (uint32_t i = 1; i < s_Data.TexIndex; i++)
				{
					if (texture->GetRenderID() == s_Data.TexSlots[i]->GetRenderID())
					{
						texIndex = (float)i;
						break;
					}
				}
				if (texIndex == 0.0f)
				{
					s_Data.TexSlots[s_Data.TexIndex] = texture;
					texIndex = (float)s_Data.TexIndex;
					s_Data.TexIndex++;
				}
			}
		}

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TexCoord = minTexCoord;
		s_Data.QuadVertexPtr->TexSlot = texIndex;
		s_Data.QuadVertexPtr->TexTiling = 1.0f;
		s_Data.QuadVertexPtr->EntityID = EntityID;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TexCoord = { maxTexCoord.x, minTexCoord.y };
		s_Data.QuadVertexPtr->TexSlot = texIndex;
		s_Data.QuadVertexPtr->TexTiling = 1.0f;
		s_Data.QuadVertexPtr->EntityID = EntityID;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TexCoord = maxTexCoord;
		s_Data.QuadVertexPtr->TexSlot = texIndex;
		s_Data.QuadVertexPtr->TexTiling = 1.0f;
		s_Data.QuadVertexPtr->EntityID = EntityID;
		s_Data.QuadVertexPtr++;

		s_Data.QuadVertexPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexPtr->Color = color;
		s_Data.QuadVertexPtr->TexCoord = { minTexCoord.x, maxTexCoord.y };
		s_Data.QuadVertexPtr->TexSlot = texIndex;
		s_Data.QuadVertexPtr->TexTiling = 1.0f;
		s_Data.QuadVertexPtr->EntityID = EntityID;
		s_Data.QuadVertexPtr++;

		s_Data.IndicesCount += 6;
	}
	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.IndicesCount);
		s_statics.drawCallTime++;
	}

	void Renderer2D::StartBatch()
	{
		s_Data.IndicesCount = 0;
		//Ptr指向该数组
		s_Data.QuadVertexPtr = s_Data.QuadVertexBase;
	}
}