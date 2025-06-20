#pragma once
#include "vertexConstantBuffer.h"

class ConstantPixelBuffer : public ConstantVertexBuffer
{
public:
	void bind() override
	{
		static ConstantPixelBuffer* last = nullptr;
		if (last != this)
		{
			last = this;
			getContext()->PSSetConstantBuffers(bufferSlot, 1, constantBuffer.GetAddressOf());
		}
	}
};