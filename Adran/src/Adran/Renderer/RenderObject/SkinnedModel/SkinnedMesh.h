#pragma once

#include<vector>
#include "SkinnedVertex.h"
#include "../Material.h"
#include "../VertexArray.h"
namespace Adran
{
	class SkinnedMesh
	{
	public:
		SkinnedMesh(
			const std::vector<SkinnedVertex>& vertices,
			const std::vector<uint32_t>& indices,
			const Ref<Material>& material = nullptr
		) :m_vertices(vertices), m_indices(indices), m_material(material)
		{
			SetupMesh();
		}

		inline Ref<Material> GetMaterial() const { return m_material; }
		inline Ref<VertexArray> GetVertexArray() const { return m_vertexArray; }

	private:
		void SetupMesh();
		void MoveMesh(SkinnedMesh&& other);
	private:
		std::vector<SkinnedVertex> m_vertices;
		std::vector<uint32_t> m_indices;
		Ref<Material> m_material;
	private:
		Ref<VertexArray> m_vertexArray;
		Ref<VertexBuffer> m_vertexBuffer;
		Ref<IndexBuffer> m_indexBuffer;
	};
}