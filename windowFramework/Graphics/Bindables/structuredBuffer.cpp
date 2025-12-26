#include "structuredBuffer.h"


void StructuredBuffer::create(const void* data, int _arraySize, int _objectSize)
{
	arraySize = (UINT)_arraySize;
	objectSize = (UINT)_objectSize;

	offset = 0;

	// Creates buffer
	D3D11_SUBRESOURCE_DATA subresource = { data };

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = objectSize * arraySize;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = objectSize;


	_throwHr(getDevice()->CreateBuffer(&bufferDesc, &subresource, &buffer));

	// Creates buffer view
	//criando a view da textura
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_UNKNOWN;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	viewDesc.Buffer.FirstElement = 0;
	viewDesc.Buffer.NumElements = arraySize;

	_throwHr
	(
		getDevice()->CreateShaderResourceView(buffer.Get(), &viewDesc, bufferView.GetAddressOf())
	);


	initialized = true;
}

void StructuredBuffer::update(const void* data)
{
	D3D11_MAPPED_SUBRESOURCE msr = {};
	_throwHr(
		getContext()->Map(buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr)
	);

	memcpy(msr.pData, data, objectSize * arraySize);

	getContext()->Unmap(buffer.Get(), 0u);
}

void StructuredBuffer::setSlot(int n)
{
	bufferSlot = n;
}

void StructuredBuffer::bind()
{
	static StructuredBuffer* last = nullptr;
	if (last != this)
	{
		last = this;
		getContext()->VSSetShaderResources(bufferSlot, 1, bufferView.GetAddressOf());
	}
}