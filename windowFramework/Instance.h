#pragma once
#include <DirectXMath.h>
#include "vec3.h"

class Instance
{
public:
	Instance(vec3 Position, vec3 Angle);

	void update(vec3 Position, vec3 Angle);
	
	void move(vec3 Position, vec3 Angle);

	DirectX::XMMATRIX getMatrix();

private:
	vec3 position;
	vec3 angle;
};