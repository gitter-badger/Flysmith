#include "PCH.h"
#include "SwapChain.h"
using namespace cuc;


SwapChain::SwapChain()
	: m_bInitialized(false)
	, m_pSwapChain(nullptr)
	, m_indexLastBufSwap(0)
{
}

SwapChain::~SwapChain()
{
	m_pSwapChain->Release();
}

void SwapChain::Init(ID3D12CommandQueue* pCommandQueue, const HWND windowHandle)
{
	assert(pCommandQueue != nullptr);

	IDXGIFactory2* pDXGIFactory = nullptr;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pDXGIFactory));
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

	m_bInitialized = true;
}

void SwapChain::Present(const bool bVsync)
{
	assert(m_bInitialized);

	U32 syncInterval = 1 ? bVsync : 0;
	m_pSwapChain->Present(syncInterval, 0);
	
	m_indexLastBufSwap = (1 + m_indexLastBufSwap) % 2;
}

void SwapChain::Resize(const U32 width, const U32 height)
{
	m_pSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
}

void SwapChain::GetBuffer(ID3D12Resource** ppResource)
{
	assert(m_bInitialized);
	assert(ppResource != nullptr);

	HRESULT hr = m_pSwapChain->GetBuffer(m_indexLastBufSwap, __uuidof(ID3D12Resource), reinterpret_cast<void**>(ppResource));
	assert(SUCCEEDED(hr));
}