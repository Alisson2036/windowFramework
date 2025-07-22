#include "pixelShader.h"



void PixelShader::create(const wchar_t* _shaderFile)
{
	shaderFile = _shaderFile;

	//criando o pixel shader
	_throwHr(D3DReadFileToBlob(shaderFile, pixelShaderBlob.GetAddressOf()));

	createShader();

	initialized = true;
}

void PixelShader::create(const Microsoft::WRL::ComPtr<ID3DBlob>& blob)
{
	pixelShaderBlob = blob;

	createShader();

	initialized = true;
}


void PixelShader::bind()
{
	static PixelShader* last = nullptr;
	if (last != this)
	{
		last = this;
		getContext()->PSSetShader(pixelShader.Get(), nullptr, 0);
	}
}

Microsoft::WRL::ComPtr<ID3DBlob> PixelShader::getBlob()
{
	return pixelShaderBlob;
}

void PixelShader::createShader()
{
	_throwHr(
		getDevice()->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize(),
			nullptr,
			&pixelShader
		)
	);
}