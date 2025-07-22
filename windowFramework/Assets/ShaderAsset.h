#pragma once
#include <string>
#include <filesystem>
#include "IAsset.h"
#include "..\Resources\image.h"
#include "..\Graphics\Shader\shader.h"

class ShaderAsset : public IAsset
{
public:
    ShaderAsset(const std::filesystem::path& path);

    void Load() override;
    void Unload() override;
    bool IsLoaded() const override;
    const std::filesystem::path& GetFilePath() const override;
    const std::string& GetFileName() const override;

    Shader* getTexture();

private:

    std::filesystem::path filePath; // Caminho do arquivo
    std::string fileName;           // Nome do arquivo
    bool loaded = false;            // Indica se o asset está carregado
};