#include "pixelShader.h"



void PixelShader::create(const wchar_t* _shaderFile)
{
	shaderFile = _shaderFile;

	//criando o pixel shader
	_throwHr(D3DReadFileToBlob(shaderFile, pixelShaderBlob.GetAddressOf()));


	_throwHr(getDevice()->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader));

	initialized = true;
}

void PixelShader::bind()
{
	getContext()->PSSetShader(pixelShader.Get(), nullptr, 0);
}

Microsoft::WRL::ComPtr<ID3DBlob> PixelShader::getBlob()
{
	return pixelShaderBlob;
}
