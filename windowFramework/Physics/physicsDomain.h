#pragma once
#include <vector>
#include <DirectXMath.h>
#include "physicsObject.h"
#include "..\Math\vec3.h"


class physicsDomain
{
public:
	//construtores
	physicsDomain() = default;
	physicsDomain(physicsDomain&) = delete;
	void operator=(physicsDomain&) = delete;

	//setter functions
	void addObject(physicsObject* obj);
	void setGravity(vec3 grav);

	//temp function for cube collision test
	vec3 cubeColliding(vec3 pos, vec3 rot, vec3 scale1, vec3 scale2);


	//solving functions
	void solve(float dt);

private:
	std::vector<physicsObject*> objects;
	vec3 gravity;

};