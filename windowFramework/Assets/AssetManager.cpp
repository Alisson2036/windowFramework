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

void AssetManager::LoadAll()
{
    for (auto& i : assets)
    {
        i.second->Load();
    }
}

void AssetManager::Clear()
{
    assets.clear();
}

AssetId AssetManager::getNextId()
{
    if (freeIds.empty()) return biggestId++;
    AssetId id = freeIds.front();
    freeIds.pop();
    return id;
}

void AssetManager::removeId(const AssetId& id)
{
    freeIds.push(id);
}
