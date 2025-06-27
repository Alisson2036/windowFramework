#include "AssetManager.h"

template<typename T, typename ...Args>
T* AssetManager::CreateAsset(const std::string& name, Args && ...args)
{
    static_assert(std::is_base_of_v<IAsset, T>, "T deve herdar de IAsset");
    auto asset = std::make_unique<T>(std::forward<Args>(args)...);
    T* assetPtr = asset.get();
    assets[name] = std::move(asset);
    return assetPtr;
}

IAsset* AssetManager::GetAsset(const std::string& name) const
{
    auto it = assets.find(name);
    if (it != assets.end())
        return it->second.get();
    return nullptr;
}

void AssetManager::RemoveAsset(const std::string& name)
{
    assets.erase(name);
}

void AssetManager::Clear()
{
    assets.clear();
}
