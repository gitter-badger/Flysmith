#pragma once
#include "PCH.h"
#include <dxgi1_4.h>
#include "Descriptors\DescriptorHeap.h"


class SwapChain
{
public:
	SwapChain();
	~SwapChain();
	void Init(ID3D12Device*, ID3D12CommandQueue*, const HWND);
	void Present(ID3D12Device*, const bool bVsync = true);
	void Resize(const U32 width, const U32 height);

	ID3D12Resource* GetRenderTarget();
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVLocation();

private:
	void CreateRTV(ID3D12Device*);

	IDXGISwapChain1* m_pSwapChain;
	U32 m_indexLastBufSwap;

	DescriptorHeap m_rtDescHeap;
	ID3D12Resource* m_pRenderTarget;
};