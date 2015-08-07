#pragma once
#include "Resources\Resource.h"
#include "Descriptors\IndexBufferView.h"
#include "Descriptors\VertexBufferView.h"
#include "Mesh.h"


namespace cuc
{
	class MeshRenderData
	{
	public:
		void Init(ID3D12Device* pDevice, Mesh& mesh)
		{
			assert(pDevice != nullptr);

			m_vertBuffer.Reset();
			auto vertBufSize = mesh.verts.size() * sizeof(Vertex);
			ResourceConfig descVBuf(ResourceType::BUFFER, vertBufSize);
			m_vertBuffer.CreateCommited(pDevice, descVBuf, nullptr, &mesh.verts[0], vertBufSize);

			m_indexBuffer.Reset();
			auto indexBufSize = mesh.indices.size() * sizeof(U32);
			ResourceConfig descIBuf(ResourceType::BUFFER, indexBufSize);
			m_indexBuffer.CreateCommited(pDevice, descIBuf, nullptr, &mesh.indices[0], indexBufSize);

			// Create resource descriptors
			m_vertBufferView.Init(m_vertBuffer.GetGPUVirtualAddress(), vertBufSize, sizeof(Vertex));
			m_indexBufferView.Init(m_indexBuffer.GetGPUVirtualAddress(), indexBufSize);

			m_numIndices = mesh.indices.size();
		}

		VertexBufferView& GetVertBufferView()
		{
			return m_vertBufferView;
		}

		IndexBufferView& GetIndexBufferView()
		{
			return m_indexBufferView;
		}

		U32 GetNumIndices()
		{
			return m_numIndices;
		}

	private:
		Resource m_vertBuffer;
		VertexBufferView m_vertBufferView;

		Resource m_indexBuffer;
		IndexBufferView m_indexBufferView;
		U32 m_numIndices;
	};
}