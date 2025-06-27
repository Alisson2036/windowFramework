#include "MeshAsset.h"

MeshAsset::MeshAsset(const std::filesystem::path& path)
{
	filePath = path;
	fileName = path.filename().string();

}

bool MeshAsset::Load()
{
    return false;
}

void MeshAsset::Unload()
{
}

bool MeshAsset::IsLoaded() const
{
    return false;
}

const std::filesystem::path& MeshAsset::GetFilePath() const
{
    // TODO: inserir instrução return aqui
}

const std::string& MeshAsset::GetFileName() const
{
    // TODO: inserir instrução return aqui
}

void MeshAsset::getData(inputBuffer* out)
{
}
