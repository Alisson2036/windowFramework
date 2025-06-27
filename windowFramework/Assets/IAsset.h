#pragma once
#include <filesystem>
#include <string>

class IAsset
{
public:
    virtual ~IAsset() = default;

    virtual bool Load() = 0;
    virtual void Unload() = 0;
    virtual bool IsLoaded() const = 0;
    virtual const std::filesystem::path& GetFilePath() const = 0;
    virtual const std::string& GetFileName() const = 0;
};  