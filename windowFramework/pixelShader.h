#pragma once
#include <wrl.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "exception.h"
#include "bindable.h"

class PixelShader : Bindable
{
public:

	void create(const wchar_t* _shaderFile);

	void bind() override;
	void setDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device) override
	{
		device = _device;
	}
	void setContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext) override
	{
		context = _deviceContext;
	}

	Microsoft::WRL::ComPtr<ID3DBlob> getBlob();

private:


	const wchar_t* shaderFile;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  pixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob>           pixelShaderBlob;

};