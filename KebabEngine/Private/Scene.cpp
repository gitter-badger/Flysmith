#include "PCH.h"
#include "Scene.h"


Scene::Scene()
{
	for (size_t entityIndex = 0; entityIndex < MAX_ENTITIES; entityIndex++)
	{
		entities[entityIndex].m_id = entityIndex;
	}
}