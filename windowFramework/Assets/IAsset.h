#pragma once
#include <filesystem>
#include <string>

class IAsset
{
public:
    virtual ~IAsset() = default;

    // Métodos puros que as classes derivadas devem implementar
    virtual bool Load(const std::filesystem::path& path) = 0;
    virtual void Unload() = 0;
    virtual bool IsLoaded() const = 0;
    virtual const std::filesystem::path& GetFilePath() const = 0;
    virtual const std::string& GetFileName() const = 0;
};  