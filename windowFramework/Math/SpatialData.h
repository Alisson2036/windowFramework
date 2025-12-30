#pragma once
#include <DirectXMath.h>
#include "vec3.h"

class SpatialData
{
public:
	SpatialData();
	SpatialData(vec3 Position, vec3 Angle);

	void set(vec3 Position, vec3 Angle);
	
	void move(vec3 Position);

	void rotate(vec3 Angle);

	void setScale(vec3 Scale);

	DirectX::XMMATRIX getMatrix();
	vec3 getPosition();

private:
	vec3 scale;
	vec3 position;
	vec3 angle;
};