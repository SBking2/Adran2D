#include "AdranPreCompile.h"
#include "SkinnedMesh.h"

namespace Adran
{
	void SkinnedMesh::SetupMesh()
	{
		m_vertexArray = VertexArray::Create();

		m_vertexBuffer = VertexBuffer::Create(
			reinterpret_cast<float*>(m_vertices.data()),
			m_vertices.size() * sizeof(SkinnedVertex)
		);
		m_vertexBuffer->SetLayout(SkinnedVertex::GetLayout());
		m_indexBuffer = Adran::IndexBuffer::Create(
			m_indices.data(),
			m_indices.size()
		);
		m_vertexArray->AddVertexBuffer(m_vertexBuffer);
		m_vertexArray->SetIndexBuffer(m_indexBuffer);

		m_vertexArray->UnBind();
	}
	void SkinnedMesh::MoveMesh(SkinnedMesh&& other)
	{
		m_vertices = std::move(other.m_vertices);
		m_indices = std::move(other.m_indices);
		m_material = std::move(other.m_material);

		m_vertexArray = std::move(other.m_vertexArray);
		m_vertexBuffer = std::move(other.m_vertexBuffer);
		m_indexBuffer = std::move(other.m_indexBuffer);
	}
}