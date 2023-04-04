#pragma once
#include "vertexConstantBuffer.h"

class ConstantPixelBuffer : public ConstantVertexBuffer
{
public:
	void bind() override
	{
		getContext()->PSSetConstantBuffers(bufferSlot, 1, constantBuffer.GetAddressOf());
	}
};