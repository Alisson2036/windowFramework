#pragma once
#include <d3dcompiler.h>
#include <wrl.h>
#include <filesystem>
#include <string>
#include "../../Core/exception.h"


class ShaderCompiler
{
public:
	enum class ShaderType
	{
		PixelShader,
		VertexShader
	};
	ShaderCompiler(const std::filesystem::path& file, const ShaderType& type);

	void compile();
	void saveToFile(const std::filesystem::path& file);

	Microsoft::WRL::ComPtr<ID3DBlob> getCompiledBlob();


private:

	std::filesystem::path filePath; // Caminho do arquivo
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob = nullptr;
	ShaderType shaderType;
	UINT size;

};