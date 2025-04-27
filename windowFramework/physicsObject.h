#pragma once
#include <cmath>
#include "vec3.h"

class physicsObject
{
public:
	//constructors
	physicsObject() = default;
	physicsObject(vec3 position);

	//setters
	void setPosition(vec3 position);

	//getters
	vec3 getPosition();

	//solve functions
	void solveVerlet(const vec3 accel, const float dt);
	void containInBox(const vec3 boxPos, const vec3 boxSize);
	void collide(physicsObject* obj);


private:
	vec3 pos;
	vec3 lastPos;
	float radius = 1.0f;
};