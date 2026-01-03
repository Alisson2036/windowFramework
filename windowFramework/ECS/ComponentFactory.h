#pragma once
#include "Components\Object.h"
#include "..\Graphics\Shader\inputBuffer.h"
#include "Registry.h"


#include "..\Assets\MeshAsset.h"
#include "..\Assets\MaterialAsset.h"
#include "..\Assets\TextureAsset.h"
#include "..\Math\SpatialData.h"



class ComponentFactory
{
public:
	ComponentFactory(Registry* registry);


	Entity createObject(
		MaterialAsset* materialAsset,
		MeshAsset* meshAsset,
		SpatialData initialPos
	);



private:


	Registry* reg;

};