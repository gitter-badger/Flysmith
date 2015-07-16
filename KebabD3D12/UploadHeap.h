#pragma once
#include "PCH.h"


namespace cuc
{
	class UploadHeap
	{
	public:
		enum Type
		{
			BUFFERS_AND_SINGLE_SAMPLE_TEX,
			MULTI_SAMPLE_TEX
		};
	
	public:
		UploadHeap();
		~UploadHeap();
		void Init(ID3D12Device*, const U64 size, const Type = BUFFERS_AND_SINGLE_SAMPLE_TEX);
		
		void Alloc(ID3D12Resource**, const D3D12_RESOURCE_DESC&, void* data, const size_t dataSize, 
				   const D3D12_RESOURCE_STATES = D3D12_RESOURCE_STATE_GENERIC_READ, const D3D12_CLEAR_VALUE* = nullptr);

		ID3D12Heap* Get();

	private:
		ID3D12Heap* m_pHeap;
		U64 m_currentOffset;
		U64 m_size;
		Type m_type;
		bool m_bInitialized;
		ID3D12Device* m_pDevice;

		static const std::map<Type, U64> s_heapAlignment;
	};
}