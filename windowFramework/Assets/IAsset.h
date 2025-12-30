#pragma once
#include <filesystem>
#include <string>

class IAsset
{
public:
    virtual ~IAsset() = default;

    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual bool IsLoaded() const = 0;
};  