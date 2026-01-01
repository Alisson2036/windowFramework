#include "MaterialAsset.h"

void MaterialAsset::Load()
{
	for (auto& i : textureAssets)
		i.second->Load();
}

void MaterialAsset::Unload()
{
	for (auto& i : textureAssets)
		i.second->Unload();
}

bool MaterialAsset::IsLoaded() const
{
	if (!shader->IsLoaded()) return false;
	for (auto& i : textureAssets)
		if (!i.second->IsLoaded()) return false;
	for (auto& i : textureOnly)
		if (!i.second->isInitialized()) return false;
	return true;

}

void MaterialAsset::addTexture(TextureAsset* texAsset, UINT slot)
{
	clearSlot(slot);

	textureAssets.push_back(std::pair(slot, texAsset));

}

void MaterialAsset::addTexture(Texture* tex, UINT slot)
{
	clearSlot(slot);

	textureOnly.push_back(std::pair(slot, tex));
}

void MaterialAsset::clearSlot(UINT slot)
{
	std::erase_if(
		textureAssets,
		[&](std::pair<UINT, TextureAsset*> pair) {
			return pair.first == slot;
		}
	); 
	std::erase_if(
		textureOnly,
		[&](std::pair<UINT, Texture*> pair) {
			return pair.first == slot;
		}
	);
}

TextureAsset* MaterialAsset::getTextureAsset(UINT slot)
{
	auto i = std::find_if(
		textureAssets.begin(),
		textureAssets.end(),
		[&](std::pair<UINT, TextureAsset*> pair) {
			return pair.first == slot;
		}
	);
	
	return i == textureAssets.end() ? nullptr : i._Ptr->second;
}

ShaderAsset* MaterialAsset::getShader()
{
	return shader;
}

Texture* MaterialAsset::getTexture(UINT slot)
{
	auto i = std::find_if(
		textureOnly.begin(),
		textureOnly.end(),
		[&](std::pair<UINT, Texture*> pair) {
			return pair.first == slot;
		}
	);

	return i == textureOnly.end() ? nullptr : i._Ptr->second;
}

void MaterialAsset::bindMaterial()
{
	shader->getShader()->bind();

	for (auto& i : textureAssets)
	{
		auto* t = i.second->getTexture();
		t->setSlot(i.first);
		t->bind();
	}
	for (auto& i : textureOnly)
	{
		i.second->setSlot(i.first);
		i.second->bind();
	}
}
