#pragma once
#include <wrl.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "exception.h"
#include "bindable.h"


class IndexBuffer : public Bindable
{
public:
	//deve ser composto por shorts
	void create(D3D11_SUBRESOURCE_DATA data, int arraySize);

	void bind() override;

private:
	//vertex buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;


};

