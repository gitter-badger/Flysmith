#include "DX12Renderer.h"
#ifdef RENDERER_DX12
#include <d3dcompiler.h>
#include "Application\Window.h"
#include "DX12\SwapChainConfig.h"
using namespace cuc;
using namespace Microsoft::WRL;


DX12Renderer::DX12Renderer(const std::shared_ptr<Window>& pWindow)
	: m_pWindow(pWindow)
{
	LoadPipeline();
	LoadAssets();
}

DX12Renderer::~DX12Renderer()
{
}

HRESULT DX12Renderer::CreateDeviceAndSwapChain(const D3D_DRIVER_TYPE driverType, const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc)
{
	ComPtr<ID3D12Device>       pDevice;
	ComPtr<IDXGIFactory1>      pDxgiFactory;
	ComPtr<IDXGISwapChain>     pDxgiSwapChain;
	ComPtr<ID3D12CommandQueue> pQueue;

	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&pDevice);
	if (FAILED(hr))
	{
		// TODO: Log error
		return hr;
	}

	D3D12_COMMAND_QUEUE_DESC queueDesc;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	hr = pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pQueue));
	if (FAILED(hr))
	{
		// TODO: Log error
		return hr;
	}

	hr = CreateDXGIFactory1(IID_PPV_ARGS(&pDxgiFactory));
	if (FAILED(hr))
	{
		// TODO: Log error
		return hr;
	}

	DXGI_SWAP_CHAIN_DESC localSwapChainDesc = *pSwapChainDesc;
	hr = pDxgiFactory->CreateSwapChain(pQueue.Get(), &localSwapChainDesc, &pDxgiSwapChain);
	if (FAILED(hr))
	{
		// TODO: Log error
		return hr;
	}

	hr = pDevice.Get()->QueryInterface(IID_PPV_ARGS(m_pDevice.GetAddressOf()));
	if (FAILED(hr))
	{
		// TODO: Log error
		return hr;
	}

	hr = pQueue.Get()->QueryInterface(IID_PPV_ARGS(m_pCommandQueue.GetAddressOf()));
	if (FAILED(hr))
	{
		// TODO: Log error
		return hr;
	}

	hr = pDxgiSwapChain.Get()->QueryInterface(IID_PPV_ARGS(m_pSwapChain.GetAddressOf()));
	if (FAILED(hr))
	{
		// TODO: Log error
		reinterpret_cast<IUnknown*>(m_pDevice.GetAddressOf())->Release();
		return hr;
	}

	return S_OK;
}

void DX12Renderer::LoadPipeline()
{
	SwapChainConfig swapChainConfig(m_pWindow->GetHandle(), true, { 1, 0 });

	HRESULT hr = CreateDeviceAndSwapChain(D3D_DRIVER_TYPE_HARDWARE, &swapChainConfig.GetDescritpion());
	if (FAILED(hr))
	{
		// TODO: Log error
		CreateDeviceAndSwapChain(D3D_DRIVER_TYPE_WARP, &swapChainConfig.GetDescritpion());
	}

	hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_pCommandAllocator.GetAddressOf()));
	if (FAILED(hr))
	{
		// TODO: Log error
	}
}

void DX12Renderer::LoadAssets()
{
	ComPtr<ID3DBlob> blobShaderVert;
	ComPtr<ID3DBlob> blobShaderPixel;

	D3DCompileFromFile(L"testShader.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, blobShaderVert.GetAddressOf(), nullptr);
	D3DCompileFromFile(L"testShader.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, blobShaderPixel.GetAddressOf(), nullptr);

	D3D12_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, 
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Root signature
	D3D12_ROOT_SIGNATURE_DESC descRootSignature;
	descRootSignature.NumParameters = 0;
	descRootSignature.pParameters = nullptr;
	descRootSignature.NumStaticSamplers = 0;
	descRootSignature.pStaticSamplers = nullptr;
	descRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	
	ComPtr<ID3DBlob> pOutBlob, pErrorBlob;
	D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, pOutBlob.GetAddressOf(), pErrorBlob.GetAddressOf());
	m_pDevice->CreateRootSignature(0, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(), IID_PPV_ARGS(pOutBlob.GetAddressOf()));
}

void DX12Renderer::Render()
{

}

#endif