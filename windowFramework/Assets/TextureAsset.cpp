#include "TextureAsset.h"


TextureAsset::TextureAsset(Image&& img)
	:
	m_img(std::move(img)),
	mipMap(false)
{}

TextureAsset::TextureAsset(const std::filesystem::path& path, bool generateMipMap)
{
	filePath = path;
	fileName = path.filename().string();
	mipMap = generateMipMap;
}

void TextureAsset::Load()
{
	if (filePath.empty() && !m_img.isLoaded()) return;
	if (!m_img.isLoaded())
	{
		m_img.loadFile(filePath.c_str());
	}
	if (mipMap) tex.createWithMipMap(m_img);
	else tex.create(m_img);
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
