#include "vertexShader.h"



void VertexShader::create(const wchar_t* _shaderFile)
{
	shaderFile = _shaderFile;

	_throwHr(D3DReadFileToBlob(shaderFile, vertexShaderBlob.GetAddressOf()));


	createShader();

	initialized = true;
}

void VertexShader::create(const Microsoft::WRL::ComPtr<ID3DBlob>& blob)
{
	vertexShaderBlob = blob;

	createShader();

	initialized = true;
}

void VertexShader::bind()
{
	static VertexShader* last = nullptr;
	if (last != this)
	{
		getContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
	}
}

Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::getBlob()
{
	return vertexShaderBlob;
}

void VertexShader::createShader()
{
	_throwHr(
		getDevice()->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			nullptr,
			&vertexShader
		)
	);
}
