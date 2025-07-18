#include "vertexShader.h"



void VertexShader::create(const wchar_t* _shaderFile)
{
	shaderFile = _shaderFile;

	//criando o pixel shader
	_throwHr(D3DReadFileToBlob(shaderFile, vertexShaderBlob.GetAddressOf()));


	_throwHr(getDevice()->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader));

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
