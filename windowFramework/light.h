#pragma once
#include <DirectXMath.h>
#include "pixelConstantBuffer.h"


class Light
{
public:
	Light();

	void updatePos(float _pos[3]);

	void bind(int bufferSlot);

private:
	float pos[3] = { 0.0f, 1.0f, 0.0f };
	ConstantPixelBuffer buf;
};