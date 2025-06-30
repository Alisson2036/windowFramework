#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
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

    /**
     * Carrega dados manualmente para uma sem�ntica espec�fica do shader.
     * @tparam T Tipo do dado (ex: vec3, vec2, color)
     * @param semanticName Nome da sem�ntica do shader (ex: "POSITION", "COLOR", "TEXCOORD")
     * @param data Vetor de dados a ser carregado
     */
    template<typename T>
    void manualLoad(const std::string& semanticName, const std::vector<T>& data)
    {
        meshData[semanticName].stride = static_cast<char>(sizeof(T));
        meshData[semanticName].buffer.resize(data.size() * sizeof(T));
        // C�pia dos dados.
        std::memcpy(meshData[semanticName].buffer.data(), data.data(), data.size() * sizeof(T));
        // Atualiza quantidade de v�rtices.
        if(vCount < data.size())
            vCount = data.size();
        loaded = true;
        // Impede que o arquivo seja recarregado incorretamente.
        fileName.clear();
        filePath.clear();
    }

private:
    struct data
    {
        char stride;
        std::vector<char> buffer;
    };

    std::filesystem::path filePath; // Caminho do arquivo
    std::string fileName;           // Nome do arquivo
    bool loaded = false;            // Indica se o asset est� carregado

    std::unordered_map<std::string, data> meshData; // Dados do mesh
    size_t vCount = 0u;                             // Quantidade total de vertices
};