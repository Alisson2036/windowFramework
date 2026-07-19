#pragma once
#include "vertexConstantBuffer.h"

template <typename T>
class ConstantPixelBuffer : public ConstantVertexBuffer<T>
{
public:
	void bind() override
	{
		
		this->getContext()->PSSetConstantBuffers(this->bufferSlot, 1, this->constantBuffer.GetAddressOf());
	}
};