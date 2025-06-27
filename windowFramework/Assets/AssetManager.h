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

    // Cria e armazena um asset do tipo T, retornando ponteiro para o asset criado
    template<typename T, typename... Args>
    T* CreateAsset(const std::string& name, Args&&... args);

    // Recupera um asset pelo nome
    IAsset* GetAsset(const std::string& name) const;

    // Remove um asset pelo nome
    void RemoveAsset(const std::string& name);

    // Limpa todos os assets
    void Clear();

private:
    std::unordered_map<std::string, std::unique_ptr<IAsset>> assets;
};


