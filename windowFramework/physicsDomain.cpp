#include "physicsDomain.h"

void physicsDomain::addObject(physicsObject* obj)
{
	objects.push_back(obj);
}

void physicsDomain::setGravity(vec3 grav)
{
	gravity = grav;
}

void physicsDomain::solve(float dt)
{
	const vec3 boxPos(-10.0f, 20.0f, -10.0f);
	const vec3 boxSize(20.f, -20.f, 20.f);

	//solve verlet for all objects
	//and contain inside box
	for (physicsObject* i : objects)
	{
		i->solveVerlet(gravity, dt);
		i->containInBox(boxPos, boxSize);
	}

	//collisions
	for(int i = 0; i < objects.size(); i++)
		for (int j = i + 1; j < objects.size(); j++)
			objects[i]->collide(objects[j]);
}
