#include "PCH.h"
#include "ResourceRegistry.h"


void ResourceRegistry::AddResource(const std::string& name, ResourceHandle handle)
{
	m_resources[name] = handle;
}

ResourceHandle ResourceRegistry::GetHandle(const std::string& name)
{
	assert(m_resources.find(name) != m_resources.end());
	return m_resources[name];
}