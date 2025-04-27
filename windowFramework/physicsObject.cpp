#include "physicsObject.h"

physicsObject::physicsObject(vec3 position)
{
	setPosition(position);
}

void physicsObject::setPosition(vec3 position)
{
	pos = position;
	lastPos = position;
}

vec3 physicsObject::getPosition()
{
	return pos;
}

void physicsObject::solveVerlet(vec3 accel, float dt)
{
	vec3 newPos = (pos * 2) - lastPos + ( accel*(dt*dt) );
	lastPos = pos;
	pos = newPos;
}

void physicsObject::containInBox(const vec3 boxPos, const vec3 boxSize)
{
	if (pos.y < 1.0f)
	{
		pos.y -= (pos.y - 1.0f);
	}
}
