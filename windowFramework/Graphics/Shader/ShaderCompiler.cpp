#include "ShaderCompiler.h"



ShaderCompiler::ShaderCompiler(const std::filesystem::path& file, const ShaderType& type)
{
	filePath = file;
	shaderType = type;
}

void ShaderCompiler::compile()
{

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	std::string type;
	switch (shaderType)
	{
	case ShaderCompiler::ShaderType::PixelShader:
		type = "ps_5_0";
		break;
	case ShaderCompiler::ShaderType::VertexShader:
		type = "vs_5_0";
		break;
	default:
		break;
	}

	HRESULT hr = D3DCompileFromFile(
		filePath.c_str(),
		NULL, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		type.c_str(),
		flags, 
		0, 
		&shaderBlob, 
		NULL
	);

	_throwHr(hr);
	if (!shaderBlob.Get())
		_throwMsg("Shader compilation error");
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderCompiler::getCompiledBlob()
{
	return shaderBlob;
}
