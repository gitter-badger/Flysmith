#include "PCH.h"
#include "SwapChain.h"


SwapChain::SwapChain()
	: m_pSwapChain(nullptr)
	, m_indexLastBufSwap(0)
	, m_pRenderTarget(nullptr)
{
}

SwapChain::~SwapChain()
{
	if (m_pSwapChain)
		m_pSwapChain->Release();

	if (m_pRenderTarget)
		m_pRenderTarget->Release();
}

void SwapChain::Init(ID3D12Device* pDevice, ID3D12CommandQueue* pCommandQueue, const HWND windowHandle)
{
	assert(pCommandQueue != nullptr);
	assert(pDevice != nullptr);

	IDXGIFactory2* pDXGIFactory = nullptr;
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&pDXGIFactory));
	assert(SUCCEEDED(hr));

	DXGI_SWAP_CHAIN_DESC1 desc;
	desc.Width = 0;   // Get from output window
	desc.Height = 0;  // Get from output window
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Stereo = FALSE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = pDXGIFactory->CreateSwapChainForHwnd(pCommandQueue, windowHandle, &desc, nullptr, nullptr, &m_pSwapChain);
	assert(SUCCEEDED(hr));

	pDXGIFactory->Release();

	// Create render target
	m_rtDescHeap.Init(pDevice, DescHeapType::RENDER_TARGET, 1);
	CreateRTV(pDevice);
}

void SwapChain::Present(ID3D12Device* pDevice, const bool bVsync)
{
	assert(pDevice != nullptr);
	assert(m_pRenderTarget != nullptr);

	U32 syncInterval = 1 ? bVsync : 0;
	m_pSwapChain->Present(syncInterval, 0);
	
	m_indexLastBufSwap = (1 + m_indexLastBufSwap) % 2;

	CreateRTV(pDevice);
}

void SwapChain::Resize(const U32 width, const U32 height)
{
	m_pSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
}

ID3D12Resource* SwapChain::GetRenderTarget()
{
	assert(m_pRenderTarget != nullptr);
	return m_pRenderTarget;
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetRTVLocation()
{
	return m_rtDescHeap.GetCPUHandle(0);
}

void SwapChain::CreateRTV(ID3D12Device* pDevice)
{
	assert(pDevice != nullptr);

	HRESULT hr = m_pSwapChain->GetBuffer(m_indexLastBufSwap, IID_PPV_ARGS(&m_pRenderTarget));
	assert(SUCCEEDED(hr));
	pDevice->CreateRenderTargetView(m_pRenderTarget, nullptr, m_rtDescHeap.GetCPUHandle(0));
}
