#include "indexBuffer.h"

void IndexBuffer::create(const void* data, int arraySize)
{
	D3D11_SUBRESOURCE_DATA subresource = { data };

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(int) * arraySize;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	_throwHr(
		getDevice()->CreateBuffer(
			&bufferDesc,
			&subresource,
			&indexBuffer
		)
	);

	initialized = true;
}

void IndexBuffer::bind()
{
	static IndexBuffer* last = nullptr;
	if (last != this)
	{
		last = this;
		getContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}
