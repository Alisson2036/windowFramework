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
	Camera* cam;                     // c�mera de proje��o da luz, usada para
									 // calcular a matriz de proje��o da sombra
	vec3 pos = { 0.0f, 1.0f, 0.0f };
	ConstantPixelBuffer buf;         // Pixel constant buffer para posi��o da luz
	ConstantVertexBuffer matBuf;     // Vertex constant buffer para a matriz de proje��o da luz
};