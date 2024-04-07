#pragma once
#include "../VertexArray.h"
#include "../Buffer.h"
#include "../Material.h"
#include "Vertex.h"

namespace Adran
{

	class Mesh
	{
	public:
		Mesh(
			const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices,
			const Ref<Material>& material = nullptr
		):m_vertices(vertices), m_indices(indices), m_material(material)
		{
			SetupMesh();
		}

		inline Ref<Material> GetMaterial() const { return m_material; }
		inline Ref<VertexArray> GetVertexArray() const {  return m_vertexArray; }
	private:
		void SetupMesh();
		void MoveMesh(Mesh&& other);
	private:
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		Ref<Material> m_material;
	private:
		Ref<VertexArray> m_vertexArray;
		Ref<VertexBuffer> m_vertexBuffer;
		Ref<IndexBuffer> m_indexBuffer;
	};
}