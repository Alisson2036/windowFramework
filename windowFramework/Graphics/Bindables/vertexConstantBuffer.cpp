#include "vertexConstantBuffer.h"



void ConstantVertexBuffer::create(const void* data, int _arraySize, int _objectSize)
{
	arraySize = (UINT)_arraySize;
	objectSize = (UINT)_objectSize;
	if ((arraySize * objectSize) % 16 != 0)
		_throwMsg("constant buffer should have a size that is a multiple of 16");
	offset = 0;

	D3D11_SUBRESOURCE_DATA subresource = { data };

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = objectSize * arraySize;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	_throwHr(getDevice()->CreateBuffer(&constantBufferDesc, &subresource, &constantBuffer));
	initialized = true;
}

void ConstantVertexBuffer::update(const void* data)
{
	D3D11_MAPPED_SUBRESOURCE msr = {};
	_throwHr(
		getContext()->Map(constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr)
	);

	memcpy(msr.pData, data, objectSize * arraySize);

	getContext()->Unmap(constantBuffer.Get(), 0u);
}

void ConstantVertexBuffer::setSlot(int n)
{
	bufferSlot = n;
}

void ConstantVertexBuffer::bind()
{
	static ConstantVertexBuffer* last = nullptr;
	if (last != this)
	{
		last = this;
		getContext()->VSSetConstantBuffers(bufferSlot, 1, constantBuffer.GetAddressOf());
	}
}
