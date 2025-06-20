#pragma once
#include <DirectXMath.h>
#include "Bindables\pixelConstantBuffer.h"
#include "Bindables\vertexConstantBuffer.h"
#include "..\Math\vec3.h"
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