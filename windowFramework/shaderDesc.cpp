#include "shaderDesc.h"

ShaderDesc::ShaderDesc(Microsoft::WRL::ComPtr<ID3DBlob> blob)
{
	//criando o reflect do shader
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflect;

	//refletindo o shader
	_throwHr(
		D3DReflect(blob->GetBufferPointer(),
			blob->GetBufferSize(),
			__uuidof(ID3D11ShaderReflection),
			(void**)reflect.GetAddressOf()
		)
	);

	//pegando a descricao do shader
	D3D11_SHADER_DESC desc = {};
	reflect->GetDesc(&desc);

	//lendo todos os constantBuffers do shader
	for (unsigned int i = 0; i < desc.ConstantBuffers; i++)
	{
		ID3D11ShaderReflectionConstantBuffer* buf = reflect->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufDesc = {};
		buf->GetDesc(&bufDesc);
		ID3D11ShaderReflectionVariable* var = buf->GetVariableByIndex(0);
		

	}

	//lendo inputLayout
	for (unsigned int i = 0; i < desc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC parDesc;
		reflect->GetInputParameterDesc(i, &parDesc);
		inputParams.push_back((std::string)parDesc.SemanticName);

	}

}
