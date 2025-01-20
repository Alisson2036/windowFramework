#pragma once
#include <DirectXMath.h>
#include "vec3.h"

class Position3d
{
public:
	Position3d();
	Position3d(vec3 Position, vec3 Angle);

	void set(vec3 Position, vec3 Angle);
	
	void move(vec3 Position);

	void rotate(vec3 Angle);

	void setScale(vec3 Scale);

	DirectX::XMMATRIX getMatrix();

private:
	vec3 scale;
	vec3 position;
	vec3 angle;
};