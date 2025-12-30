#pragma once
#include <string>
#include <filesystem>
#include <concepts>
#include <vector>

#include "IAsset.h"
#include "..\Resources\image.h"
#include "..\Graphics\Shader\shader.h"
#include "..\Graphics\Shader\ShaderCompiler.h"
#include "TextureAsset.h"
#include "ShaderAsset.h"


class MaterialAsset : public IAsset
{
public:

    template<typename... TextureAssets>
    requires (std::same_as<TextureAssets, TextureAsset> && ...)
    MaterialAsset(
        ShaderAsset* shaderAsset,
        TextureAssets*... textures
    )
        : shader(shaderAsset)
    {
        UINT slot = 0;

        // Expande as texturas e salva os slots de maneira incremental
        ( (textureAssets.emplace_back(slot++, textures)), ... );
    }

    void Load() override;
    void Unload() override;
    bool IsLoaded() const override;

    void addTexture(TextureAsset* texAsset, UINT slot);
    void addTexture(Texture* tex, UINT slot);
    void clearSlot(UINT slot);

    TextureAsset* getTextureAsset(UINT slot);
    Texture* getTexture(UINT slot);

    void bindMaterial();
    

private:

    ShaderAsset* shader = nullptr;

    std::vector<std::pair<UINT, TextureAsset*>> textureAssets;
    std::vector<std::pair<UINT, Texture*>> textureOnly;
    
    bool loaded = false; // Indica se o asset está carregado
};