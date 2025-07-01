#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include "IAsset.h"
#include "..\Resources\image.h"
#include "..\Graphics\Bindables\texture.h"

class TextureAsset : public IAsset
{
public:
    TextureAsset(const std::filesystem::path& path, bool generateMipMap);

    void Load() override;
    void Unload() override;
    bool IsLoaded() const override;
    const std::filesystem::path& GetFilePath() const override;
    const std::string& GetFileName() const override;

    const Texture* getTexture() const;

private:

    std::filesystem::path filePath; // Caminho do arquivo
    std::string fileName;           // Nome do arquivo
    bool loaded = false;            // Indica se o asset está carregado

    Texture tex; // Gpu-side texture
    Image img;   // Cpu-side texture
    bool mipMap; // Needs MipMap generation
};