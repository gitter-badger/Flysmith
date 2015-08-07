#include "PCH.h"
#include "Mesh.h"
using namespace cuc;


Mesh::Mesh()
{
}

Mesh::Mesh(Mesh&& other)
{
	// TODO: Check if @other is initialized

	verts = other.verts;
	indices = other.indices;
	m_vertBufferView = other.m_vertBufferView;
	m_indexBufferView = other.m_indexBufferView;

	other.m_vertBuffer.Get()->AddRef();
	m_vertBuffer = other.m_vertBuffer;

	other.m_indexBuffer.Get()->AddRef();
	m_indexBuffer = other.m_indexBuffer;
}

Mesh& Mesh::operator=(Mesh&& other)
{
	// TODO: Check if @other is initialized

	verts = other.verts;
	indices = other.indices;
	m_vertBufferView = other.m_vertBufferView;
	m_indexBufferView = other.m_indexBufferView;

	other.m_vertBuffer.Get()->AddRef();
	m_vertBuffer = other.m_vertBuffer;

	other.m_indexBuffer.Get()->AddRef();
	m_indexBuffer = other.m_indexBuffer;

	return *this;
}

void Mesh::Init(ID3D12Device* pDevice)
{
	assert(pDevice != nullptr);

	m_vertBuffer.Reset();
	auto vertBufSize = verts.size() * sizeof(Vertex);
	ResourceConfig descVBuf(ResourceType::BUFFER, vertBufSize);
	m_vertBuffer.CreateCommited(pDevice, descVBuf, nullptr, &verts[0], vertBufSize);

	m_indexBuffer.Reset();
	auto indexBufSize = indices.size() * sizeof(U32);
	ResourceConfig descIBuf(ResourceType::BUFFER, indexBufSize);
	m_indexBuffer.CreateCommited(pDevice, descIBuf, nullptr, &indices[0], indexBufSize);

	// Create resource descriptors
	m_vertBufferView.Init(m_vertBuffer.GetGPUVirtualAddress(), vertBufSize, sizeof(Vertex));
	m_indexBufferView.Init(m_indexBuffer.GetGPUVirtualAddress(), indexBufSize);
}

VertexBufferView& Mesh::GetVertBufferView()
{
	return m_vertBufferView;
}

IndexBufferView& Mesh::GetIndexBufferView()
{
	return m_indexBufferView;
}

U32 Mesh::GetNumIndices()
{
	return indices.size();
}