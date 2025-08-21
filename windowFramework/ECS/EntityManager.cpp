#include "EntityManager.h"

Entity EntityManager::createEntity()
{
	if (freeEntities.empty())
		return biggestEntity += 1u;
	Entity id = freeEntities.front();
	freeEntities.pop();
	return id;
}

void EntityManager::removeEntity(Entity ent)
{
	freeEntities.push(ent);
}
