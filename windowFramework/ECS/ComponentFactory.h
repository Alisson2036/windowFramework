#pragma once
#include "Components\Object.h"
#include "..\Graphics\Shader\inputBuffer.h"
#include "Registry.h"


#include "..\Assets\MeshAsset.h"
#include "..\Assets\ShaderAsset.h"
#include "..\Assets\TextureAsset.h"



class ComponentFactory
{
public:
	ComponentFactory(Registry* registry);


	Entity createObject(
		ShaderAsset* shaderAsset,
		MeshAsset* meshAsset
	);



private:


	Registry* reg;

};