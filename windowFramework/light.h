#pragma once
#include <DirectXMath.h>
#include "pixelConstantBuffer.h"
#include "vertexConstantBuffer.h"
#include "vec3.h"
#include "Camera.h"


class Light
{
public:
	Light();

	void updatePos(vec3 Position);
	void setShadowMapProjectionCam(Camera* _cam);

	void bind(int pixelBufferSlote, int vertexBufferSlot);

private:

private:
	Camera* cam;
	vec3 pos = { 0.0f, 1.0f, 0.0f };
	ConstantPixelBuffer buf;
	ConstantVertexBuffer matBuf;
};