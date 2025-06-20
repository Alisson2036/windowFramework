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
	vec3 boxEnd = boxPos + boxSize;

	//y
	if (pos.y < boxEnd.y + radius)
		pos.y += boxEnd.y + radius - pos.y;
	if (pos.y > boxPos.y - radius)
		pos.y -= pos.y - boxPos.y + radius;

	//x
	if (pos.x < boxPos.x + radius)
		pos.x += boxPos.x + radius - pos.x;
	if (pos.x > boxEnd.x - radius)
		pos.x -= pos.x - boxEnd.x + radius;

	//z
	if (pos.z < boxPos.z + radius)
		pos.z += boxPos.z + radius - pos.z;
	if (pos.z > boxEnd.z - radius)
		pos.z -= pos.z - boxEnd.z + radius;
}

void physicsObject::collide(physicsObject* obj)
{
	vec3 delta = pos - obj->pos;
	float distSquared =
		(delta.x * delta.x) +
		(delta.y * delta.y) +
		(delta.z * delta.z);
	if (distSquared < std::pow(radius + obj->radius, 2))
	{
		float dist = std::sqrt(distSquared);
		vec3 deltaNorm = delta / dist;
		float offset = dist - (radius + obj->radius);
		pos -= deltaNorm * offset * 0.5f;
		obj->pos += deltaNorm * offset * 0.5f;
	}
}
