#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "bindable.h"
#include "vertexShader.h"
#include "exception.h"



class InputLayout : Bindable
{
public:
	void create(VertexShader vs, std::vector<D3D11_INPUT_ELEMENT_DESC>elementDescription, D3D11_PRIMITIVE_TOPOLOGY _topology);

	void bind() override;
	void setDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device) override
	{
		device = _device;
	}
	void setContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext) override
	{
		context = _deviceContext;
	}
	



private:
	D3D11_PRIMITIVE_TOPOLOGY topology;
	//D3D11_INPUT_ELEMENT_DESC* elementDescription;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

};