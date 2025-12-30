#pragma once
#include <string>
#include <filesystem>
#include "IAsset.h"
#include "..\Resources\image.h"
#include "..\Graphics\Shader\shader.h"
#include "..\Graphics\Shader\ShaderCompiler.h"

class ShaderAsset : public IAsset
{
public:
    ShaderAsset(
        const std::filesystem::path& vsShader,
        const std::filesystem::path& vsCompiled,
        const std::filesystem::path& psShader,
        const std::filesystem::path& psCompiled
    );

    void Load() override;
    void Unload() override;
    bool IsLoaded() const override;
    const std::filesystem::path& GetFilePath() const;
    const std::string& GetFileName() const;

    Shader* getShader();

private:

    std::filesystem::path vsShaderFile;   // Caminho do arquivo
    std::filesystem::path psShaderFile;   // Caminho do arquivo
    std::filesystem::path vsCompiledFile; // Caminho do arquivo
    std::filesystem::path psCompiledFile; // Caminho do arquivo

    Shader shader;       // Shader interno
    bool loaded = false; // Indica se o asset está carregado
};