#include "TextureAsset.h"


TextureAsset::TextureAsset(const std::filesystem::path& path)
{
	filePath = path;
	fileName = path.filename().string();
}

void TextureAsset::Load()
{
}

void TextureAsset::Unload()
{
}

bool TextureAsset::IsLoaded() const
{
	return loaded;
}

const std::filesystem::path& TextureAsset::GetFilePath() const
{
	return filePath;
}

const std::string& TextureAsset::GetFileName() const
{
	return fileName;
}
