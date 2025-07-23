#include "ShaderAsset.h"

ShaderAsset::ShaderAsset(const std::filesystem::path& vsShader, const std::filesystem::path& vsCompiled, const std::filesystem::path& psShader, const std::filesystem::path& psCompiled)
{
	vsShaderFile = vsShader;
	psShaderFile = psShader;
	vsCompiledFile = vsCompiled;
	psCompiledFile = psCompiled;
}

void ShaderAsset::Load()
{

	ShaderCompiler vs(vsShaderFile, ShaderCompiler::ShaderType::VertexShader);
	ShaderCompiler ps(psShaderFile, ShaderCompiler::ShaderType::PixelShader);

	vs.compile();
	ps.compile();

	shader.create(vs.getCompiledBlob(), ps.getCompiledBlob());

	loaded = true;
}

void ShaderAsset::Unload()
{
}

bool ShaderAsset::IsLoaded() const
{
	return loaded;
}

const std::filesystem::path& ShaderAsset::GetFilePath() const
{
	return "";
}

const std::string& ShaderAsset::GetFileName() const
{
	return "";
}

Shader* ShaderAsset::getShader()
{
	return &shader;
}
