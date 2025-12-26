#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "..\..\Core\exception.h"
#include "bindable.h"





class StructuredBuffer : public Bindable
{
public:
	void create(const void* data, int arraySize, int objectSize);

	void update(const void* data);

	void setSlot(int n);

	void bind() override;


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bufferView;

	//tamanho de cada vertice em bytes
	UINT arraySize = 0;
	UINT objectSize = 0;
	UINT offset = 0;

	//slot do constant buffer
	int bufferSlot = 0;

};

