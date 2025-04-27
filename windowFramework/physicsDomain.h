#pragma once
#include <vector>
#include "physicsObject.h"
#include "vec3.h"


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


	//solving functions
	void solve(float dt);

private:
	std::vector<physicsObject*> objects;
	vec3 gravity;

};