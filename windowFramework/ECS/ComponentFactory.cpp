#include "ComponentFactory.h"

ComponentFactory::ComponentFactory(Registry* registry)
{
	reg = registry;
}

Entity ComponentFactory::createObject(ShaderAsset* shaderAsset, MeshAsset* meshAsset)
{

	// Creates entity
	Entity ent = reg->createEntity();


	// Transform component
	SpatialData pos;
	pos.set(
		{ 0.f,0.f,0.f },
		{ 0.f,0.f,0.f }
	);

	reg->addComponent(ent, pos);


	// Material Component
	CMaterial material;
	material.shader = shaderAsset->getShader();
	material.textures = {};

	reg->addComponent(ent, material);

	// Mesh component
	CMeshNonIndexed mesh;
	mesh.mesh = meshAsset;
	mesh.shader = shaderAsset;
	mesh.Textures = {};
	
	{
		// Creates vertex buffer
		inputBuffer ib;
		ib.create(material.shader->inputParams);
		meshAsset->getData(&ib);
		ib.createVertexBuffer(mesh.vb);
		mesh.vertexCount = ib.getElementCount();
	}
	

	reg->addComponent(ent, mesh);

	



	return ent;
}
