#pragma once
#include <memory>

#include "..\..\Assets\MeshAsset.h"
#include "..\..\Assets\MaterialAsset.h"
#include "..\..\Assets\TextureAsset.h"
#include "..\..\Math\SpatialData.h"

#include "..\..\Graphics\Bindables\vertexConstantBuffer.h"
#include "..\..\Graphics\Bindables\indexBuffer.h"




struct CMeshNonIndexed
{
	MaterialAsset* material;
	MeshAsset* mesh;
};




