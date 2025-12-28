#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "..\..\Core\exception.h"
#include "bindable.h"




template<typename T>
class StructuredBuffer : public Bindable
{
public:
	void create(const T* data, int _arraySize)
	{
		currentArraySize = (UINT)_arraySize;

		// Creates buffer
		D3D11_SUBRESOURCE_DATA subresource = { data };

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(T) * currentArraySize;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = sizeof(T);

		if (data == nullptr)
		{
			_throwHr(getDevice()->CreateBuffer(&bufferDesc, NULL, &buffer));
		}
		else
		{

			_throwHr(getDevice()->CreateBuffer(&bufferDesc, &subresource, &buffer));
		}
		// Creates buffer view
		//criando a view da textura
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = DXGI_FORMAT_UNKNOWN;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		viewDesc.Buffer.FirstElement = 0;
		viewDesc.Buffer.NumElements = currentArraySize;

		_throwHr
		(
			getDevice()->CreateShaderResourceView(buffer.Get(), &viewDesc, bufferView.GetAddressOf())
		);


		initialized = true;
	}

	void update(const T* data, int arraySize)
	{
		if (arraySize > currentArraySize) _throwMsg("Allocated buffer is not big enough.");

		D3D11_MAPPED_SUBRESOURCE msr = {};
		_throwHr(
			getContext()->Map(buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr)
		);

		memcpy(msr.pData, data, sizeof(T) * arraySize);

		getContext()->Unmap(buffer.Get(), 0u);
	}

	UINT getArraySize()
	{
		return currentArraySize;
	}

	void setSlot(int n)
	{
		bufferSlot = n;
	}

	void bind() override
	{
		static StructuredBuffer* last = nullptr;
		if (last != this)
		{
			last = this;
			getContext()->VSSetShaderResources(bufferSlot, 1, bufferView.GetAddressOf());
		}
	
		getContext()->VSSetShaderResources(bufferSlot, 1, bufferView.GetAddressOf());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bufferView;

	//tamanho de cada vertice em bytes
	UINT currentArraySize = 0;

	//slot do constant buffer
	int bufferSlot = 0;

};

