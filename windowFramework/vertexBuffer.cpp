#include "vertexBuffer.h"

void VertexBuffer::create(const void* data, int arraySize, int objectSize, bool _isMutable)
{
	stride = (UINT)objectSize;
	offset = 0;
	objectCount = (UINT)arraySize;

	//release pointer caso ele ja exista
	vertexBuffer.Reset();

	D3D11_SUBRESOURCE_DATA subresource = { data };

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = stride * objectCount;
	vertexBufferDesc.Usage = _isMutable ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	_throwHr(getDevice()->CreateBuffer(&vertexBufferDesc, &subresource, &vertexBuffer));
	initialized = true;
	isMutable = _isMutable;
}

void VertexBuffer::setSlot(unsigned int n)
{
	slot = n;
}

void VertexBuffer::update(const void* data)
{
	if (!initialized) _throwMsg("Vertex Buffer has not been initialized.");
	if (!isMutable) _throwMsg("Vertex Buffer is not mutable.");

	getContext()->UpdateSubresource(
		vertexBuffer.Get(),
		0,
		nullptr,
		data,
		objectCount * stride,
		0u
	);
}

void VertexBuffer::bind()
{
	static VertexBuffer* last = nullptr;
	if (last != this)
	{
		last = this;
		getContext()->IASetVertexBuffers(slot, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	}
}
