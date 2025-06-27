#pragma once
#include <memory>
#include "IAsset.h"
#include "..\Resources\objLoader.h"
#include "..\Graphics\Shader\inputBuffer.h"

class MeshAsset : public IAsset
{
public:
    MeshAsset();
    MeshAsset(const std::filesystem::path& path);

    void Load() override;
    void Unload() override;
    bool IsLoaded() const override;
    const std::filesystem::path& GetFilePath() const override;
    const std::string& GetFileName() const override;

    /**
     * Preenche o inputBuffer fornecido com os dados do asset.
     * @param[out] out inputBuffer a ser preenchido com os dados do asset.
     */
    void getData(inputBuffer* out);

private:

    struct data
    {
        char stride;
        std::vector<char> buffer;
    };

private:
    std::filesystem::path filePath; // Caminho do arquivo
    std::string fileName;           // Nome do arquivo
    bool loaded = false;            // Indica se o asset está carregado

	std::map<std::string, data> meshData; // Dados do mesh
    size_t vCount;                        // Quantidade total de vertices

};