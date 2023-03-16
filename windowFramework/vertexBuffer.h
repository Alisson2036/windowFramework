#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "exception.h"
#include "bindable.h"


class VertexBuffer : public Bindable
{
public:
	void create(const void* data, int arraySize, int objectSize);

	void bind() override;

private:
	//vertex buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

	//tamanho de cada vertice em bytes
	UINT stride = 0;
	UINT offset = 0;

};

