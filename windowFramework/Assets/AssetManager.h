#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <filesystem>
#include "IAsset.h"
#include "..\Core\exception.h"

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

    // Retorna um pointer para o IAsset com o nome dado
    IAsset* GetAsset(const std::string& name) const;

	// Retorna um pointer para o asset do tipo T com o nome dado
    template<typename AssetType>
    AssetType* getAsset(const std::string& name) const
    {
        static_assert(std::is_base_of_v<IAsset, AssetType>, "T deve herdar de IAsset");
        
        if (auto it = assets.find(name); it != assets.end())
        {
            if (auto p = dynamic_cast<AssetType*>(it->second.get()))
                return p;
            _throwMsg("Asset required is not the correct type.");
        }
		return nullptr;
    }

    // Remove um asset pelo nome
    void RemoveAsset(const std::string& name);

    // Carrega todos os assets
    void LoadAll();

    // Limpa todos os assets
    void Clear();

private:
    std::unordered_map<std::string, std::unique_ptr<IAsset>> assets;
};


