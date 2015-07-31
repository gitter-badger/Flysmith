#include "PCH.h"
#include "ResourceBarrier.h"


cuc::TransitionBarrier::TransitionBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter, BarrierFlag flag)
{
	assert(pResource != nullptr);

	Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	Flags = static_cast<D3D12_RESOURCE_BARRIER_FLAGS>(flag);
	Transition.pResource = pResource;
	Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	Transition.StateBefore = stateBefore;
	Transition.StateAfter = stateAfter;
}

void cuc::TransitionBarrier::SetResource(ID3D12Resource* pResource)
{
	assert(pResource != nullptr);
	Transition.pResource = pResource;
}

void cuc::TransitionBarrier::SetTransition(D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
{
	Transition.StateBefore = stateBefore;
	Transition.StateAfter = stateAfter;
}

cuc::AliasingBarrier::AliasingBarrier(ID3D12Resource* pResourceBefore, ID3D12Resource* pResourceAfter, BarrierFlag flag)
{
	Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
	Flags = static_cast<D3D12_RESOURCE_BARRIER_FLAGS>(flag);
	Aliasing.pResourceBefore = pResourceBefore;
	Aliasing.pResourceAfter = pResourceAfter;
}

cuc::UAVBarrier::UAVBarrier(ID3D12Resource* pResource, BarrierFlag flag)
{
	Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	Flags = static_cast<D3D12_RESOURCE_BARRIER_FLAGS>(flag);
	UAV.pResource;
}
