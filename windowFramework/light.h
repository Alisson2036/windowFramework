#pragma once
#include <DirectXMath.h>
#include "pixelConstantBuffer.h"
#include "vec3.h"
#include "Camera.h"


class Light
{
public:
	Light();

	void updatePos(vec3 Position);
	void setLightCam(Camera* _cam);

	void bind(int bufferSlot);

private:

private:
	Camera* cam;
	vec3 pos = { 0.0f, 1.0f, 0.0f };
	ConstantPixelBuffer buf;
	ConstantPixelBuffer matBuf;
};