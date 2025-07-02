#include "TextureAsset.h"



TextureAsset::TextureAsset(const std::filesystem::path& path, bool generateMipMap)
{
	filePath = path;
	fileName = path.filename().string();
	mipMap = generateMipMap;
}

void TextureAsset::Load()
{
	if (filePath.empty()) return;

	img.loadFile(filePath.c_str());
	if (mipMap) tex.createWithMipMap(img);
	else tex.create(img);
	loaded = true;
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

Texture* TextureAsset::getTexture()
{
	return &tex;
}
