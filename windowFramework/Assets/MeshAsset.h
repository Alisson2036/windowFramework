#pragma once
#include "IAsset.h"
#include "..\Graphics\Shader\inputBuffer.h"

class MeshAsset : public IAsset
{
public:
    MeshAsset(const std::filesystem::path& path);

    bool Load() override;
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
    std::filesystem::path filePath;  // Caminho do arquivo
    std::string fileName;            // Nome do arquivo
    bool loaded = false;             // Indica se o asset está carregado
	inputBuffer meshData;            // Dados do mesh

};