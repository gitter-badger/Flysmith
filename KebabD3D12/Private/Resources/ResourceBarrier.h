#pragma once
#include "PCH.h"


namespace cuc
{
	enum class BarrierType
	{
		// Indicates a transition of a set of subresources between usages.
		// The before and after usages must be specified.
		TRANSITION = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		// Indicates a transition between usages of 2 different resources that have mappings into the same tile pool. 
		// Both the before and the after resource can be specified. 
		// One or both resources can be NULL, which indicates that any tiled resource could cause aliasing.
		ALIASING = D3D12_RESOURCE_BARRIER_TYPE_ALIASING,
		// Indicates that all UAV accesses(reads/writes) to a resource must complete before any future UAV accesses can begin. 
		UAV = D3D12_RESOURCE_BARRIER_TYPE_UAV
	};

	enum class BarrierFlag
	{
		NONE = 0,
		// Can only be used to begin a transition
		BEGIN_ONLY = 0x1, 
		// Can only be used to end a transition.
		END_ONLY = 0x2
	};

	struct TransitionBarrier : D3D12_RESOURCE_BARRIER
	{
		TransitionBarrier(ID3D12Resource*, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter, BarrierFlag = BarrierFlag::NONE);
	
		void SetResource(ID3D12Resource*);
		void SetTransition(D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);
	};

	struct AliasingBarrier : D3D12_RESOURCE_BARRIER
	{
		AliasingBarrier(ID3D12Resource* pResourceBefore, ID3D12Resource* pResourceAfter, BarrierFlag = BarrierFlag::NONE);
	};

	struct UAVBarrier : D3D12_RESOURCE_BARRIER
	{
		UAVBarrier(ID3D12Resource*, BarrierFlag = BarrierFlag::NONE);
	};
}