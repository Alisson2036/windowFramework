#include "vertexBuffer.h"

void VertexBuffer::create(const void* data, int arraySize, int objectSize)
{
	stride = (UINT)objectSize;
	offset = 0;

	D3D11_SUBRESOURCE_DATA subresource = { data };

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = stride * arraySize;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	_throwHr(device->CreateBuffer(&vertexBufferDesc, &subresource, &vertexBuffer));
	initialized = true;
}

void VertexBuffer::bind()
{
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}
