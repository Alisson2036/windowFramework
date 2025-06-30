#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <filesystem>
#include "IAsset.h"

class AssetManager
{
public:
    AssetManager() = default;
    ~AssetManager() = default;

    /**
     * Cria e armazena um asset do tipo T, e retorna um pointer para
     * o asset criado.
     * @tparam T Tipo do asset, que deve ser baseado no IAsset
     * @param name Nome do asset (não pode se repetir)
     * @param args Argumentos para a criação do asset
     */
    template<typename T, typename... Args>
    T* CreateAsset(const std::string& name, Args&&... args)
    {
        static_assert(std::is_base_of_v<IAsset, T>, "T deve herdar de IAsset");
        auto asset = std::make_unique<T>(std::forward<Args>(args)...);
        T* assetPtr = asset.get();
        assets[name] = std::move(asset);
        return assetPtr;
    }

    // Recupera um asset pelo nome
    IAsset* GetAsset(const std::string& name) const;

    // Remove um asset pelo nome
    void RemoveAsset(const std::string& name);

    // Carrega todos os assets
    void LoadAll();

    // Limpa todos os assets
    void Clear();

private:
    std::unordered_map<std::string, std::unique_ptr<IAsset>> assets;
};


