#pragma once
#include <queue>
#include "Entity.h"



class EntityManager
{
public:
	EntityManager() = default;

	Entity createEntity();
	void removeEntity(Entity ent);

private:
	std::queue<Entity> freeEntities;
	
	Entity biggestEntity; //biggest ID number


};