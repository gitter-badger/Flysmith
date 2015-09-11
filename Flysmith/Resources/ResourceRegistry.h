#pragma once
#include "Types.h"


using ResourceHandle = U32;

// Keeps track of resource handles/resource name pairings 
class ResourceRegistry
{
public:
	void AddResource(const std::string& name, ResourceHandle);
	ResourceHandle GetHandle(const std::string& name);

private:
	std::map<std::string, ResourceHandle> m_resources;
};