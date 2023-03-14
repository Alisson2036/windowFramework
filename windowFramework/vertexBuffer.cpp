#include "vertexBuffer.h"

inline void VertexBuffer::create(void* data, int arraySize, int objectSize)
{
	stride = (UINT)objectSize;

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = stride * arraySize;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData = { data };

	_throwHr(device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer));

}

inline void VertexBuffer::bind()
{
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, 0);
}
