#pragma once
#include <filesystem>
#include <cinttypes>
#include <string>

using AssetId = std::uint32_t;

class AssetManagerKey
{
    friend class AssetManager;
    AssetManagerKey() = default;
};

class IAsset
{
    friend class AssetManager;
public:
    virtual ~IAsset() = default;

    AssetId getId() const { return m_id; }
    void setId(AssetId id, AssetManagerKey) 
    { 
        m_id = id; 
    }

    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual bool IsLoaded() const = 0;
private:
    AssetId m_id;
};