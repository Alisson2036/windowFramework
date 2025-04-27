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
	const vec3 boxPos(0.f, 0.f, 0.f);
	const vec3 boxSize(0.f, 0.f, 0.f);

	//solve verlet for all objects
	for (physicsObject* i : objects)
	{
		i->solveVerlet(gravity, dt);
		i->containInBox(boxPos, boxSize);
	}
}
