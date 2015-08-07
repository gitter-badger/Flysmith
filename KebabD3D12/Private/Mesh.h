#pragma once
#include "Resources\Resource.h"
#include "Descriptors\IndexBufferView.h"
#include "Descriptors\VertexBufferView.h"
#include "Vertex.h"


namespace cuc
{
	class Mesh
	{
	public:
		Mesh();

		Mesh(Mesh&& other);
		Mesh& operator=(Mesh&& other);

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		void Init(ID3D12Device* pDevice);

		VertexBufferView& GetVertBufferView();
		IndexBufferView& GetIndexBufferView();
		U32 GetNumIndices();
		
	public:
		std::vector<Vertex> verts;
		std::vector<U32> indices;

	private:
		Resource m_vertBuffer;
		VertexBufferView m_vertBufferView;

		Resource m_indexBuffer;
		IndexBufferView m_indexBufferView;
	};
}