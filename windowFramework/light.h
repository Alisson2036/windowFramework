#pragma once
#include <DirectXMath.h>
#include "pixelConstantBuffer.h"
#include "vec3.h"


class Light
{
public:
	Light();

	void updatePos(vec3 Position);

	void bind(int bufferSlot);

private:
	vec3 pos = { 0.0f, 1.0f, 0.0f };
	ConstantPixelBuffer buf;
};