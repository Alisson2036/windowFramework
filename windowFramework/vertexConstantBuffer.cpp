#include "vertexConstantBuffer.h"



void ConstantVertexBuffer::create(const void* data, int arraySize, int objectSize)
{
	stride = (UINT)objectSize;
	offset = 0;

	D3D11_SUBRESOURCE_DATA subresource = { data };

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = stride * arraySize;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	_throwHr(device->CreateBuffer(&constantBufferDesc, &subresource, &constantBuffer));
}

void ConstantVertexBuffer::bind()
{
	context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}
