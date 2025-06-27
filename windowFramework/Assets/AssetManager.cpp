#include "AssetManager.h"

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
