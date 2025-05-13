#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "exception.h"
#include "bindable.h"


class VertexBuffer : public Bindable
{
public:
	void create(const void* data, int arraySize, int objectSize, bool _isMutable = false);

	void setSlot(unsigned int n);
	void update(const void* data);

	void bind() override;

private:
	//vertex buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

	//tamanho de cada vertice em bytes
	UINT slot = 0;
	UINT stride = 0;
	UINT objectCount = 0;
	UINT offset = 0;
	bool isMutable = false;
};

