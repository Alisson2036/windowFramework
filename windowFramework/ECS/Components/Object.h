#pragma once
#include <memory>

#include "..\..\Assets\MeshAsset.h"
#include "..\..\Assets\ShaderAsset.h"
#include "..\..\Assets\TextureAsset.h"
#include "..\..\Math\SpatialData.h"

#include "..\..\Graphics\Bindables\vertexConstantBuffer.h"
#include "..\..\Graphics\Bindables\indexBuffer.h"




struct CMeshNonIndexed
{
	ShaderAsset* shader;
	MeshAsset* mesh;
	std::vector<TextureAsset*> Textures;
};





struct CMaterial
{
	Shader* shader;
	std::map<int, Texture*> textures;
};

